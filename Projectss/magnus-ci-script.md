# MagnusCI — Slide-Synced Presentation Script

> **Format:** Written to be delivered while screen-sharing the 9-slide MagnusCI deck, slide by slide. This is your secondary project — assume the panel already has calibration on your systems depth if this follows a NexusIDE walkthrough, so move a little faster through anything that echoes familiar patterns (Redis, cgroups) and spend the saved time on what's genuinely different here: the DAG scheduler and the bot-loop bug. Target total time: ~7:30, leaving buffer in a 7-8 minute slot.
>
> **Delivery note:** Let each slide sit for a beat after advancing before you speak. The diagram carries the structure; your voice carries the reasoning.

---

## SLIDE 1 — Title (0:00 – 0:25)

*[Slide is already up — pause a second, then begin]*

> "The second project I want to cover is MagnusCI — an ephemeral, container-based CI/CD orchestration engine, also live and continuously deployed, this time on Kubernetes via K3s.
>
> I built this to get hands-on with the infrastructure underneath tools like GitHub Actions and Azure Pipelines — not configure a CI system, but actually build the orchestration engine itself."

**[ADVANCE TO SLIDE 2]**

---

## SLIDE 2 — Three CI/CD Infrastructure Problems (0:25 – 1:00)

> "Three real problems shaped this build.
>
> Bursty webhook traffic — a single push to a monorepo can trigger dozens of builds in the same second.
>
> Pipeline deadlocks — a real build has a dependency graph, not a linear list of steps, and scheduling that wrong means stages waiting on each other forever.
>
> And untrusted code execution — every build runs an arbitrary script I don't control, so it has to be isolated from the host by default, not by convention."

**[ADVANCE TO SLIDE 3]**

---

## SLIDE 3 — The Mandate: Ephemeral, Zero Host Pollution (1:00 – 1:40)

> "End to end, the pipeline looks like this: a webhook arrives, gets HMAC-verified, lands in a queue, gets scheduled, runs in an ephemeral sandbox, and streams its logs back live.
>
> The mandate underneath all of it was zero host pollution — every build is fully ephemeral, nothing persists on the host between runs, and every stage in this flow is independently swappable. That's what let me build and test each piece — verification, queuing, scheduling, sandboxing — mostly in isolation from each other."

**[ADVANCE TO SLIDE 4]**

---

## SLIDE 4 — Webhook Verification & BullMQ Backpressure (1:40 – 2:30)

> "Every build starts with a GitHub webhook, and the first problem is trust — I can't execute a script triggered by an unauthenticated request. So every webhook goes through HMAC-SHA256 verification against the raw request body, checked before any JSON parsing touches it, matching exactly what GitHub itself signs.
>
> Once verified, I don't execute inline — I enqueue with BullMQ over Redis. That's what decouples ingestion from execution: the API layer's only job is verify-and-enqueue, and a separate worker pool consumes at whatever rate the host can actually sustain. That queue also gives me stalled-job auto-reclaim if a worker dies mid-build, and worker pool isolation so one noisy job can't starve the others."

**[ADVANCE TO SLIDE 5]**

---

## SLIDE 5 — Deadlock-Free Scheduling, Isolated Execution (2:30 – 3:40)

> "Pipelines aren't linear — lint, test, and build can run independently in parallel, and deploy waits on all three. So scheduling has two parts.
>
> First, a DFS-based cycle detector walks the dependency graph before anything runs, so a circular dependency gets caught immediately with a clear diagnostic instead of deadlocking at runtime.
>
> Second, a Kahn's-algorithm-based scheduler runs every stage whose dependencies are satisfied, concurrently — that's what actually gives me the parallelism, not just a valid ordering.
>
> On the execution side, each stage runs in an ephemeral Docker sandbox with three layers of isolation: cgroup limits on memory, CPU, and PID count; the Docker socket itself is never mounted into the user's sandbox, only the trusted worker pod — that's the boundary that actually matters, since a script that reaches the Docker daemon could escape the sandbox entirely; and workspace paths are locked to the build ID, so path traversal can't escape into anything else on disk."

**[ADVANCE TO SLIDE 6]**

---

## SLIDE 6 — War Story: The Bot That Reverted Itself Forever (3:40 – 4:50)

*[Your strongest slide — slow down, let each step land]*

> "This is the bug that taught me the most, and it's not a crash — it's a feedback loop. I'd built an auto-revert bot: if a build fails on main, it automatically generates and pushes a revert commit, so the branch is never left broken.
>
> The symptom was build counts climbing steadily with no new human commits. My first guess was a flaky test causing repeated re-triggers, so I dug through the test logs — that was a dead end, the failures were unrelated and inconsistent each time, not a single flaky test repeating.
>
> The actual root cause: the revert commit is itself a push to main. That re-triggers the webhook, which starts a new build, and if that build also failed for any reason, the bot would try to revert its own revert — forever. I'd built a perfectly functional infinite loop.
>
> The fix: every incoming commit gets checked for the bot's own signature, and any webhook triggered by the bot's own commits is dropped before it ever reaches the queue. The real lesson wasn't the code — it's that any system capable of automated write-actions needs a self-recognition check built in from day one, not bolted on after you find the loop."

**[ADVANCE TO SLIDE 7]**

---

## SLIDE 7 — Measured Impact (4:50 – 5:20)

> "On testing, this is backed by four full suites — unit, integration, chaos, and end-to-end — including chaos tests that specifically try to break the system: killing workers mid-build, injecting Redis failures, verifying sandbox escape attempts actually fail.
>
> The Postgres connection pool is tuned with fail-fast acquisition limits, so a burst of concurrent webhook writes fails predictably instead of exhausting the pool for every other request — zero pool exhaustion under load.
>
> Dependency caching uses Zstandard's multi-threaded compression instead of single-threaded gzip, which speeds up cache hydration — the actual bottleneck in most CI runs.
>
> And it's deployed on K3s with rolling updates and continuous health checks — a real multi-service deployment, not a single script."

**[ADVANCE TO SLIDE 8]**

---

## SLIDE 8 — Trade-offs & Design Decisions (5:20 – 6:20)

> "A few decisions worth being upfront about.
>
> BullMQ over Kafka — Redis-native, so I avoided standing up a second broker, but it's genuinely not built for durable event replay at real scale. If I needed replayable history, Kafka would be the right call.
>
> Socket.IO over Server-Sent Events — I needed bidirectional streaming for interactive builds, and SSE is one-directional only, so that wasn't actually a close call.
>
> Zstandard over gzip — multi-threaded compression, because cache hydration is the real bottleneck in CI, not compression ratio.
>
> And Docker socket isolation — mounted only to the trusted worker pod, never the user's sandbox. That one decision is the actual security boundary; everything else is defense in depth around it."

**[ADVANCE TO SLIDE 9]**

---

## SLIDE 9 — What's Next (6:20 – 7:00)

> "The lesson underneath the bot-loop bug — any self-triggering system needs a self-recognition check from day one — is really the throughline for what's next.
>
> Right now, worker pods are long-lived and handle every build. The next step is moving to ephemeral Kubernetes Jobs, one per build, which eliminates persistent idle overhead entirely, reduces blast radius since each build gets a genuinely fresh pod boundary instead of a shared long-lived one, and lets Kubernetes' native scheduling handle both burst and idle load the same way — scaling to zero when nothing's running.
>
> That's MagnusCI end to end — the DAG scheduler, the sandbox isolation, and the loop that taught me to always ask what happens when a system acts on its own output. Happy to go deeper anywhere you'd like."

**[END — leave final slide up for Q&A]**

---

## Timing Summary

| Slide | Topic | Time |
|---|---|---|
| 1 | Title | 0:00–0:25 |
| 2 | Three Problems | 0:25–1:00 |
| 3 | The Mandate | 1:00–1:40 |
| 4 | Webhook Verification & BullMQ | 1:40–2:30 |
| 5 | DAG Scheduling & Sandbox | 2:30–3:40 |
| 6 | War Story | 3:40–4:50 |
| 7 | Measured Impact | 4:50–5:20 |
| 8 | Trade-offs | 5:20–6:20 |
| 9 | What's Next / Close | 6:20–7:00 |

**Total: ~7 minutes**, leaving a buffer inside a 7-8 minute slot for pacing variance, plus whatever time follows for Q&A.

## Delivery Reminders

1. **Slide 6 (the bot-loop war story) is your strongest moment here** — same principle as NexusIDE's buffer bug. If you're running long, cut time from slide 8 (Trade-offs) first, never the war story.
2. **If this follows a NexusIDE walkthrough in the same interview**, you don't need to re-earn credibility on things you've already shown (cgroups, Redis, isolation patterns) — move faster through slide 5's isolation section and lean into what's genuinely new: the DAG algorithm and the loop bug.
3. **Bridge naturally to Microsoft's own CI/CD surface** if it fits the conversation — Azure Pipelines and GitHub Actions runners solve exactly this class of problem (trusted orchestration around untrusted code) at a scale you're approximating here on a single VM.
4. Do one full run-through out loud with the deck live before the interview.