using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace The.Measurement.Dimensions {

    /// <summary>
    /// Represents the standard dimensional model.
    /// </summary>
    /// <remarks>
    /// Derived classes might allow for additional conversions.
    /// </remarks>
    public class DimensionalModel {

        static DimensionalModel() {
            DimensionalModel.current = new DimensionalModel();
        }

        public static DimensionalModel Current {
            get {
                return DimensionalModel.current;
            }
            set {
                DimensionalModel.current = value ?? new DimensionalModel();
            }
        }

        protected DimensionalModel() { }

    //    /**
    //     * Returns the fundamental dimension for the one specified.
    //     * If the specified dimension is a dimensional product, the dimensional
    //     * product of its fundamental dimensions is returned.
    //     * Physical quantities are considered commensurate only if their
    //     * fundamental dimensions are equals using the current physics model.
    //     *
    //     * @param dimension the dimension for which the fundamental dimension is returned.
    //     * @return <code>this</code> or a rational product of fundamental dimension.
    //     */
    //    public PhysicsDimension getFundamentalDimension(PhysicsDimension dimension) {
    //    Map<? extends PhysicsDimension, Integer> dimensions = dimension.getProductDimensions();
    //    if (dimensions == null) return dimension; // Fundamental dimension.
    //    // Dimensional Product.
    //    PhysicsDimension fundamentalProduct = PhysicsDimension.NONE;
    //    for (Map.Entry<? extends PhysicsDimension, Integer> e : dimensions.entrySet()) {
    //         fundamentalProduct = fundamentalProduct.multiply(this.getFundamentalDimension(e.getKey())).pow(e.getValue());
    //    }
    //    return fundamentalProduct;
    //}

    //    /**
    //     * Returns the dimensional transform of the specified dimension.
    //     * If the specified dimension is a fundamental dimension or
    //     * a product of fundamental dimensions the identity converter is
    //     * returned; otherwise the converter from the system unit (SI) of
    //     * the specified dimension to the system unit (SI) of its fundamental
    //     * dimension is returned.
    //     *
    //     * @param dimension the dimension for which the dimensional transform is returned.
    //     * @return the dimensional transform (identity for fundamental dimensions).
    //     */
    //    public PhysicsConverter getDimensionalTransform(PhysicsDimension dimension) {
    //    Map<? extends PhysicsDimension, Integer> dimensions = dimension.getProductDimensions();
    //    if (dimensions == null) return PhysicsConverter.IDENTITY; // Fundamental dimension.
    //    // Dimensional Product.
    //    PhysicsConverter toFundamental = PhysicsConverter.IDENTITY;
    //    for (Map.Entry<? extends PhysicsDimension, Integer> e : dimensions.entrySet()) {
    //        PhysicsConverter cvtr = this.getDimensionalTransform(e.getKey());
    //        if (!(cvtr.isLinear()))
    //            throw new UnsupportedOperationException("Non-linear dimensional transform");
    //        int pow = e.getValue();
    //        if (pow < 0) { // Negative power.
    //            pow = -pow;
    //            cvtr = cvtr.inverse();
    //        }
    //        for (int j = 0; j < pow; j++) {
    //            toFundamental = toFundamental.concatenate(cvtr);
    //        }
    //    }
    //    return toFundamental;
    //}

        /// <summary>
        /// Backing field for <see cref="Current"/>.
        /// </summary>
        private static DimensionalModel current;
    }
}
