/// <copyright file="PropertyChangedEventHandlerExtensions.cs" company="TheLib Team">
/// Copyright (C) 2012 TheLib Team (http://thelib.org). All rights reserved. 
/// 
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
///
/// - Redistributions of source code must retain the above copyright notice,
///   this list of conditions and the following disclaimer.
/// - Redistributions in binary form must reproduce the above copyright notice,
///   this list of conditions and the following disclaimer in the documentation
///   and/or other materials provided with the distribution.
/// - Neither the name of TheLib, TheLib Team, nor the names of its
///   contributors may be used to endorse or promote products derived from this
///   software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THELIB TEAM AS IS AND ANY EXPRESS OR IMPLIED
/// WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
/// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
/// EVENT SHALL THELIB TEAM BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
/// TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
/// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
/// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
/// NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/// </copyright> 
/// <author>Christoph Müller</author>
/*
 * PropertyChangedEventHandlerExtensions.cs
 * 
 * Copyright (C) 2011 - 2012 Christoph Müller. Alle Rechte vorbehalten.
 */

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.Linq.Expressions;


namespace The.Utility {

    /// <summary>
    /// This class provides extension methods for
    /// <see cref="PropertyChangedEventHandler"/>.
    /// </summary>
    public static class PropertyChangedEventHandlerExtensions {

        /// <summary>
        /// Changes a property value and raises the given 
        /// <see cref="PropertyChangedEventHandler"/> if necessary.
        /// </summary>
        /// <remarks>
        /// <para>This method uses the default <see cref="EqualityComparer"/> 
        /// of the property type to determine equality.</para>
        /// </remarks>
        /// <example>
        /// public string Name {
        ///     get { 
        ///         return this.name; 
        ///     }
        ///     set { 
        ///         this.PropertyChanged.ChangeAndNotify(ref this.name, value, 
        ///             this, "Name");
        ///     }
        /// }
        /// private string name;
        /// </example>
        /// <typeparam name="T">The type of the property.</typeparam>
        /// <param name="handler">The handler to be raised if the property
        /// value did change.</param>
        /// <param name="field">The backing field of the property to be
        /// changed.</param>
        /// <param name="value">The new property value.</param>
        /// <param name="sender">The object containing the property. This value
        /// is required for raising the event and must not be <c>null</c>.
        /// </param>
        /// <param name="propertyName">The name of the property. This value is
        /// required for raising the event and must not be <c>null</c> or
        /// empty.</param>
        /// <returns><c>true</c> if the property did change, <c>false</c>
        /// otherwise.</returns>
        public static bool ChangeAndNotify<T>(
                this PropertyChangedEventHandler handler,
                ref T field,
                T value,
                INotifyPropertyChanged sender,
                string propertyName) {
            Debug.Assert(sender != null);
            Debug.Assert(!string.IsNullOrEmpty(propertyName));

            if (!EqualityComparer<T>.Default.Equals(field, value)) {
                /* Property changed, so set new value and raise event. */
                field = value;

                if (handler != null) {
                    handler(sender, new PropertyChangedEventArgs(propertyName));
                }

                return true;

            } else {
                /* Property did not change, do not do anything. */
                return false;
            }
        }

#if !WINDOWS_PHONE
        /// <summary>
        /// Changes a property value and raises the given 
        /// <see cref="PropertyChangedEventHandler"/> if necessary.
        /// </summary>
        /// <remarks>
        /// <para>This method uses the default <see cref="EqualityComparer"/> 
        /// of the property type to determine equality.</para>
        /// <para>The implementation uses a lambda expression to define the
        /// name of the property and the property owner like described in
        /// http://www.wpftutorial.net/INotifyPropertyChanged.html. However,
        /// I think the assignment there is made at the wrong position as the
        /// event should only raised once the property is already changed.
        /// </para>
        /// <para>This method relies on parsing and evaluating the lambda
        /// expression and is therefore cool, but probably slower than the
        /// counterpart that forces the caller to specify the property name
        /// explicitly.</para>
        /// <para>This method might not work on obfuscated code as mentioned in
        /// http://www.wpftutorial.net/INotifyPropertyChanged.html, but I did
        /// not test it and I did not think about it yet.</para>
        /// </remarks>
        /// <example>
        /// public string Name {
        ///     get { 
        ///         return this.name; 
        ///     }
        ///     set { 
        ///         this.PropertyChanged.ChangeAndNotify(ref this.name, value, 
        ///             () => this.Name);
        ///     }
        /// }
        /// private string name;
        /// </example>
        /// <typeparam name="T">The type of the property.</typeparam>
        /// <param name="handler">The handler to be raised if the property
        /// value did change.</param>
        /// <param name="field">The backing field of the property to be
        /// changed.</param>
        /// <param name="value">The new property value.</param>
        /// <param name="propExpression">A lambda expression designating the
        /// property field itself, i. e. something like 
        /// <c>() => this.Property</c>. This must not be <c>null</c> and the
        /// caller is responsible for specifying the correct property.
        /// </param>
        /// <returns><c>true</c> if the property did change, <c>false</c>
        /// otherwise.</returns>
        /// <exception cref="ArgumentException">If the lambda expression does
        /// not designate a property.</exception>
        public static bool ChangeAndNotify<T>(
                this PropertyChangedEventHandler handler,
                ref T field,
                T value,
                Expression<Func<T>> propExpression) {
            Debug.Assert(propExpression != null);

            if (!EqualityComparer<T>.Default.Equals(field, value)) {
                /* Property changed, so set new value and raise event. */
                field = value;

                /* 
                 * Get body of the lamba expression to retrieve the name of 
                 * the property and the object the property belongs to. This is
                 * the sender of the event.
                 */
                MemberExpression me = propExpression.Body as MemberExpression;
                if (me == null) {
                    throw new ArgumentException(
                        Properties.Resources.ErrorPropExprBodyMissing,
                        "propExpression");
                }

                ConstantExpression ce = me.Expression as ConstantExpression;
                if (ce == null) {
                    throw new ArgumentException(
                        Properties.Resources.ErrorPropBodyNotConstant,
                        "propExpression");
                }

                LambdaExpression lambda = Expression.Lambda(ce);
                Delegate lambdaDelegate = lambda.Compile();
                object sender = lambdaDelegate.DynamicInvoke();
                string propertyName = me.Member.Name;

                if (handler != null) {
                    handler(sender, new PropertyChangedEventArgs(propertyName));
                }

                return true;

            } else {
                /* 
                 * Trivial reject in case property did not change. We do this
                 * first, because the whole reflection stuff to follow is
                 * expensive.
                 */
                return false;
            }
        }
#endif // !WINDOWS_PHONE
    }
}
