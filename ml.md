# Viva Questions and Answers — Linear Regression Assignment

This is organized to follow the assignment sections exactly, so you can revise in order: Mathematical Foundations, Feature Mapping, Regularization, Programming, and general/critical-thinking questions an examiner is likely to throw in.

---

## Section 1: Mathematical Foundations

### 1(a) Least Squares Solution

**Q1. Derive the closed-form solution for w and b in simple linear regression.**

A. We minimize the mean squared error cost:

J(w, b) = (1/n) Σ (w·xi + b − ti)²

Take partial derivatives and set them to zero.

∂J/∂b = (−2/n) Σ (ti − w·xi − b) = 0
⟹ Σti − w·Σxi − nb = 0
⟹ b = t̄ − w·x̄

∂J/∂w = (−2/n) Σ xi(ti − w·xi − b) = 0
Substituting b from above and simplifying gives:

w = Σ(xi − x̄)(ti − t̄) / Σ(xi − x̄)²

So w is the covariance of x and t divided by the variance of x, and b centers the line through the mean point (x̄, t̄).

**Q2. Why do we set the derivative to zero instead of some other value?**

A. Because MSE is a convex (bowl-shaped) function of w and b. Its minimum occurs exactly where the slope of the cost surface is zero in every direction. Since it's convex, this stationary point is guaranteed to be the global minimum, not just a local one.

**Q3. What does w represent geometrically/statistically?**

A. w is the slope of the best-fit line — it tells you how much t changes on average for a one-unit increase in x. Algebraically it equals Cov(x,t)/Var(x), i.e., correlation scaled by the ratio of standard deviations.

**Q4. Why do we use squared error instead of absolute error?**

A. Squared error is differentiable everywhere (absolute error has a kink at zero), which allows a clean closed-form solution and smooth gradients. Squared error also penalizes large errors more heavily, and it corresponds to the maximum-likelihood solution under the assumption of Gaussian-distributed noise.

**Q5. Walk through a manual example with n = 3 points.**

A. Suppose data points are (1,2), (2,3), (3,5).
x̄ = 2, t̄ = 10/3.
Numerator: (1−2)(2−10/3)+(2−2)(3−10/3)+(3−2)(5−10/3) = (−1)(−4/3) + 0 + (1)(5/3) = 4/3 + 5/3 = 3
Denominator: (1−2)² + 0 + (3−2)² = 1+0+1 = 2
w = 3/2 = 1.5
b = 10/3 − 1.5(2) = 10/3 − 3 = 1/3

---

### 1(b) Gradient Descent Derivation

**Q6. Derive ∂J/∂w and ∂J/∂b for J(w,b) = (1/n)Σ(wxi + b − ti)².**

A.
∂J/∂w = (2/n) Σ xi(wxi + b − ti)
∂J/∂b = (2/n) Σ (wxi + b − ti)

**Q7. How does gradient descent use these gradients?**

A. It updates parameters iteratively in the direction opposite to the gradient (steepest descent):

w ← w − η · ∂J/∂w
b ← b − η · ∂J/∂b

where η is the learning rate. Repeating this moves (w,b) toward the minimum of the convex cost surface.

**Q8. Why not just use the closed-form solution always? Why bother with gradient descent?**

A. The closed-form solution requires computing (XᵀX)⁻¹, which is expensive or numerically unstable for very large or high-dimensional datasets (O(D³) for a D-feature matrix inversion), and it doesn't exist if XᵀX is singular. Gradient descent scales better to large datasets, works for models without closed-form solutions (like most deep learning), and can be done in mini-batches.

**Q9. What happens if the learning rate is too large or too small?**

A. Too large: updates overshoot the minimum, and the loss can oscillate or diverge. Too small: convergence becomes extremely slow, and training may get stuck in a plateau for a long time before reaching the optimum.

**Q10. Is the loss surface for linear regression convex? Does that matter for gradient descent?**

A. Yes, MSE for linear regression is a convex quadratic function in (w,b), so gradient descent (with an appropriate learning rate) is guaranteed to converge to the global minimum, not a local one.

---

### 1(c) Multivariate Linear Regression

**Q11. Write the normal equation for multivariate regression y = Xw + b (or with bias folded in).**

A. Augment X with a column of ones so that the bias is absorbed into w. Then the cost is J(w) = (1/n)||Xw − t||². Setting the gradient to zero:

∇J = (2/n) Xᵀ(Xw − t) = 0
⟹ XᵀXw = Xᵀt
⟹ w = (XᵀX)⁻¹ Xᵀt

**Q12. Under what condition does this solution exist?**

A. XᵀX must be invertible (non-singular), which requires X to have full column rank — i.e., no feature is an exact linear combination of others, and the number of independent data points is at least equal to the number of features. If features are collinear or D > N, XᵀX is singular and we need regularization (e.g., Ridge) or a pseudo-inverse instead.

**Q13. What does it mean if X has multicollinearity?**

A. Some columns (features) are linear combinations of others, making XᵀX singular or near-singular. This makes the closed-form solution unstable — small changes in the data cause huge swings in the estimated weights. Ridge regression fixes this by adding λI to XᵀX before inverting, guaranteeing invertibility.

---

## Section 2: Feature Mapping (Polynomial Regression)

**Q14. What is feature mapping, in plain language?**

A. Feature mapping transforms the original input into a richer set of features (often nonlinear functions of the input, like powers or cross-products), so a linear model can fit a nonlinear relationship in the original space. The model stays "linear in the parameters" even though the decision boundary/curve is nonlinear in x.

**Q15. For x = (x1, x2), list all degree-2 polynomial features.**

A. 1, x1, x2, x1², x2², x1x2 (six terms total including the bias/constant term). In general, for degree-2 in D dimensions there are D(D+1)/2 + D + 1 terms (pure quadratics, cross terms, linear terms, and the constant).

**Q16. Why does feature mapping increase model capacity?**

A. Each new feature adds another dimension the model can use to fit the data, effectively adding more free parameters and enabling more complex (curved) decision surfaces. This lets the model capture nonlinear structure that a plain linear model cannot.

**Q17. Why can excessive feature mapping cause overfitting?**

A. As the number of polynomial features grows, the model has enough flexibility to fit not just the true underlying trend but also the noise in the training data. With more parameters than can be reliably estimated from a fixed amount of data, the model achieves near-zero training error but generalizes poorly to unseen data because it has memorized noise-specific fluctuations rather than the signal.

**Q18. How does the number of degree-k polynomial features grow with the number of original input dimensions D?**

A. It grows combinatorially — specifically as C(D+k, k), which is polynomial in D for fixed k but grows quickly as both D and k increase. This rapid growth is the essence of the "curse of dimensionality": data becomes sparse relative to the feature space, and overfitting risk rises sharply.

**Q19. How can you detect overfitting from your degree-2/3/5/7/9 experiments?**

A. Overfitting shows up as training error continuing to decrease (or staying near zero) while test/validation error increases past a certain degree. The train/test error curves diverge — that divergence point is what you use to pick the best-fitting polynomial degree (bias-variance tradeoff).

---

## Section 3: Regularization (Ridge and Lasso)

**Q20. Why do we regularize?**

A. To control model complexity and prevent overfitting by discouraging large weight values, which correspond to overly sensitive, wiggly fits. Regularization trades a small increase in training error (bias) for a larger reduction in variance, generally improving generalization to unseen data.

**Q21. Derive the gradient descent update rule for the regularized cost J_reg^λ(w) with per-dimension λj, and explain why it's called weight decay.**

A. The regularized cost is:

J_reg = (1/2N) Σ(y⁽ⁱ⁾ − t⁽ⁱ⁾)² + (1/2) Σ λj wj²

Taking the derivative with respect to wj:

∂J_reg/∂wj = (1/N) Σ (y⁽ⁱ⁾ − t⁽ⁱ⁾) xj⁽ⁱ⁾ + λj wj

Gradient descent update:

wj ← wj − η [ (1/N) Σ(y⁽ⁱ⁾−t⁽ⁱ⁾)xj⁽ⁱ⁾ + λj wj ]
= (1 − ηλj) wj − η(1/N) Σ(y⁽ⁱ⁾−t⁽ⁱ⁾)xj⁽ⁱ⁾

Since there's no penalty on the bias:

b ← b − η (1/N) Σ(y⁽ⁱ⁾ − t⁽ⁱ⁾)

This is called "weight decay" because, ignoring the data-fit gradient term for a moment, each update multiplies wj by a factor (1 − ηλj) < 1 — the weight literally decays toward zero at every step, independent of the data term.

**Q22. Derive Ajj' and cj for the system ∂J_reg/∂wj = Σ Ajj'·wj' − cj = 0 (bias dropped, y = Σ wj xj).**

A. With the bias term dropped, y⁽ⁱ⁾ = Σ_k wk xk⁽ⁱ⁾. Expanding the squared-error term:

∂J_reg/∂wj = (1/N) Σᵢ xj⁽ⁱ⁾ (Σ_k wk xk⁽ⁱ⁾ − t⁽ⁱ⁾) + λj wj
= Σ_k [ (1/N) Σᵢ xj⁽ⁱ⁾xk⁽ⁱ⁾ ] wk + λj wj − (1/N) Σᵢ xj⁽ⁱ⁾ t⁽ⁱ⁾

So matching to Σ Ajj'wj' − cj = 0:

Ajk = (1/N) Σᵢ xj⁽ⁱ⁾ xk⁽ⁱ⁾ + λj·[j = k] (i.e., the empirical feature covariance plus λj added only on the diagonal)
cj = (1/N) Σᵢ xj⁽ⁱ⁾ t⁽ⁱ⁾

**Q23. Based on part (b), give the matrix form A, c and the closed-form solution for w.**

A. In matrix form:

A = (1/N) XᵀX + Λ, where Λ = diag(λ1, ..., λD)
c = (1/N) Xᵀt

Setting Aw = c:

w = [(1/N) XᵀX + Λ]⁻¹ (1/N) Xᵀt
= (XᵀX + N·Λ)⁻¹ Xᵀt

When all λj = λ (same for every dimension), this reduces to the standard Ridge solution:

w = (XᵀX + Nλ I)⁻¹ Xᵀt

**Q24. Why does adding λI to XᵀX always make it invertible (for λ>0)?**

A. XᵀX is always positive semi-definite (its eigenvalues are ≥ 0). Adding λI shifts every eigenvalue up by λ > 0, making all eigenvalues strictly positive, so the matrix becomes positive definite and therefore invertible — even if the original XᵀX was singular due to collinearity or D > N.

**Q25. What is the difference between Ridge (L2) and Lasso (L1) regularization?**

A. Ridge penalizes the sum of squared weights (λΣwj²); it shrinks all weights smoothly toward zero but rarely makes them exactly zero, since the penalty's gradient vanishes as wj→0. Lasso penalizes the sum of absolute weights (λΣ|wj|); because the L1 penalty has a constant-magnitude gradient (a "kink" at zero), it can drive many weights to exactly zero, producing sparse models that effectively perform feature selection.

**Q26. Why can't we derive a closed-form solution for Lasso the way we did for Ridge?**

A. The L1 penalty |wj| is not differentiable at wj = 0, so we can't simply set the gradient to zero and solve a linear system. Lasso requires iterative optimization methods such as coordinate descent or subgradient/proximal gradient methods (e.g., soft-thresholding), rather than a single matrix-inversion formula.

**Q27. Geometrically, why does Lasso produce sparse solutions but Ridge doesn't?**

A. The L1 constraint region (in weight space) is a diamond (cross-polytope) with sharp corners on the axes, while the L2 constraint region is a smooth sphere/ellipse. The optimal point (intersection of the error contours with the constraint region) is much more likely to land exactly on a corner of the diamond — where one or more weights are exactly zero — whereas the sphere has no corners, so the optimum for Ridge typically has all weights nonzero but small.

**Q28. In your Table-1 experiment (Q4d), what did you observe about the parameter magnitudes across plain, Ridge, and Lasso regression, and what does it mean?**

A. (Answer will depend on your actual run, but the expected pattern is:) Plain linear regression on the chosen polynomial degree tends to produce the largest-magnitude coefficients, especially for higher-order terms, since it has no penalty to constrain them. Ridge shrinks all coefficients toward zero but keeps them nonzero. Lasso shrinks aggressively and sets several higher-order coefficients exactly to zero, effectively selecting a simpler polynomial. This shows regularization directly trading off fit-to-training-data against model simplicity/generalization.

**Q29. Why might driving coefficients to exactly zero (Lasso) be desirable?**

A. It gives an interpretable, sparse model — you can tell which features/terms actually matter. It also reduces variance and the chance of overfitting since irrelevant or redundant features are removed automatically, rather than merely shrunk.

**Q30. How do you choose λ in practice?**

A. Via cross-validation: try a range of λ values, fit on training folds, evaluate on held-out validation folds, and pick the λ that minimizes validation error (or use something like a validation-curve / grid search, as you likely did in Q4c/4e for polynomial degree and again for λ).

---

## Section 4: Programming

**Q31. Why implement closed-form regression with "only numpy" first before using scikit-learn?**

A. To demonstrate understanding of the underlying linear algebra (matrix inversion, normal equations) rather than treating the model as a black box, and to validate scikit-learn's results against your own implementation as a sanity check.

**Q32. In gradient descent vs. the analytical (closed-form) solution for Table 1, what should you expect to see, and why might they differ slightly?**

A. With enough iterations and a suitable learning rate, gradient descent should converge to (nearly) the same w, b as the closed-form solution, since both are solving the same convex optimization problem. Small differences arise from finite iterations, learning rate, or numerical precision; gradient descent gives an approximate answer that improves as iterations increase, while the closed-form solution is exact (up to floating-point precision).

**Q33. Why did you plot loss vs. iteration, and what should a healthy loss curve look like?**

A. It visualizes convergence behavior. A healthy curve decreases monotonically and flattens out as it approaches the minimum. A curve that oscillates or increases indicates too large a learning rate; a curve that decreases extremely slowly indicates too small a learning rate.

**Q34. How did you split data 80/20 for the polynomial experiments, and why is that necessary?**

A. Training data is used to fit the parameters; a held-out test set (unseen during fitting) evaluates generalization. This split is necessary because training error alone can't detect overfitting — a high-degree polynomial can achieve near-zero training error yet generalize poorly, which only shows up as high test error.

**Q35. How did you select the "best" polynomial degree in 4(c)?**

A. By comparing training vs. testing error/loss across degrees 2, 3, 5, 7, 9, and choosing the degree where test error is minimized — this is the point that best balances underfitting (too simple, high bias) and overfitting (too complex, high variance).

**Q36. For the Boston housing dataset (4e), why is feature scaling necessary before Ridge/Lasso?**

A. Regularization penalizes the magnitude of weights uniformly. If features are on very different scales (e.g., one ranges 0–1 and another 0–10000), the corresponding weights will naturally be on different scales too, so the penalty would unfairly shrink weights for large-scale features more/less than intended. Standardizing (zero mean, unit variance) makes the penalty apply fairly across all features.

**Q37. What is RMSE and R², and how do you interpret them?**

A. RMSE (root mean squared error) is the square root of average squared prediction error, in the same units as the target — lower is better, and it's sensitive to large errors/outliers. R² (coefficient of determination) measures the proportion of variance in the target explained by the model, ranging up to 1 (perfect fit); an R² near 0 means the model is no better than predicting the mean, and negative R² means it's worse than that.

**Q38. Comparing your own coded Ridge/Lasso to scikit-learn's implementation, what should match and what might differ?**

A. The learned coefficients and predictions should be very close (ideally nearly identical) if both use the same regularization definition and converge properly, since it's the same convex optimization problem. Differences can arise because scikit-learn's Lasso uses coordinate descent with specific convergence tolerances, or because of differences in how λ is scaled/parameterized (e.g., sklearn's `alpha` may relate to your λ by a factor of N or 2N depending on convention) — so you should double check the loss normalization before comparing λ values directly.

**Q39. As λ increases, what do you expect for the coefficient magnitude plot, and why?**

A. Coefficient magnitudes should shrink toward zero as λ increases, since a larger penalty more strongly discourages large weights. For Lasso, you should additionally see coefficients hit exactly zero one by one as λ grows (a sparsity path), while Ridge coefficients shrink smoothly but rarely hit exactly zero.

---

## Section 5: Critical Thinking / General Conceptual Questions

**Q40. What are the core assumptions of linear regression?**

A. Linearity of the relationship between features and target (or after feature mapping), independent and identically distributed errors, homoscedasticity (constant error variance), no perfect multicollinearity among features, and (for inference/statistical guarantees) normally distributed residuals.

**Q41. What is the bias-variance tradeoff, and how does it relate to this assignment?**

A. Bias is the error from overly simplistic assumptions (underfitting); variance is the error from sensitivity to the specific training data (overfitting). Increasing polynomial degree reduces bias but increases variance; regularization (Ridge/Lasso) reduces variance at the cost of some bias. The "best" model minimizes the sum of these two sources of error, which is exactly what your train/test curves and λ selection are meant to find.

**Q42. What are the limitations of linear regression, even with feature mapping and regularization?**

A. It assumes the target is a (possibly nonlinearly-mapped) linear combination of features, which may not capture arbitrary complex relationships as well as more flexible models (trees, neural networks). It's sensitive to outliers (squared-error loss). Feature mapping must be manually engineered/chosen — it doesn't learn representations automatically. Regularization introduces its own hyperparameter (λ) that needs tuning, and the choice of L1 vs L2 has to match assumptions about whether true coefficients are sparse.

**Q43. Between Lasso and Ridge, when would you prefer one over the other?**

A. Prefer Lasso when you suspect only a subset of features truly matter (sparse ground truth) or want automatic feature selection/interpretability. Prefer Ridge when you believe most features contribute a little (dense ground truth), or when features are highly correlated — Ridge tends to spread weight across correlated features whereas Lasso arbitrarily picks one and zeroes out the rest, which can be less stable.

**Q44. What is Elastic Net, and why might it be useful (likely follow-up question)?**

A. Elastic Net combines L1 and L2 penalties (λ1Σ|wj| + λ2Σwj²). It gets sparsity like Lasso while handling correlated features more stably like Ridge (grouping correlated features together instead of arbitrarily picking one), making it a practical middle ground.

**Q45. If asked to extend this to a completely new dataset, what steps would you follow end-to-end?**

A. Explore and clean the data (missing values, outliers), split into train/test (or train/val/test), scale/standardize features, choose a feature mapping if the relationship looks nonlinear, fit plain/Ridge/Lasso models while tuning λ (and polynomial degree) via cross-validation on the validation set, evaluate final performance (RMSE, R²) on the held-out test set, and interpret which features/coefficients matter and why.

---

*Tip for the viva itself: examiners often ask you to re-derive one gradient or closed-form solution live on paper/whiteboard — make sure you can reproduce Q1, Q6, Q11, Q21–Q23 from memory, not just recognize the answer.*