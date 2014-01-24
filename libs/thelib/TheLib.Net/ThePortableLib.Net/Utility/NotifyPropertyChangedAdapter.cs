/// <copyright file="NotifyPropertyChangedAdapter.cs" company="TheLib Team">
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
 * NotifyPropertyChangedAdapter.cs
 * 
 * Copyright (C) 2011 Christoph Müller. Alle Rechte vorbehalten.
 */

using System.ComponentModel;


namespace The.Utility {

    /// <summary>
    /// Adapter class implementing <see cref="INotifyPropertyChanged"/>.
    /// </summary>
    public abstract class NotifyPropertyChangedAdapter
            : INotifyPropertyChanged {

        /// <inheritdoc />
        public event PropertyChangedEventHandler PropertyChanged;

        /// <summary>
        /// Changes a property value and raises the 
        /// <see cref="PropertyChanged"/> event if necessary.
        /// </summary>
        /// <remarks>
        /// <para>This method uses the default <see cref="EqualityComparer"/> 
        /// of the property type to determine equality.</para>
        /// </remarks>
        /// <typeparam name="T">The type of the property.</typeparam>
        /// <param name="field">The backing field of the property to be
        /// changed.</param>
        /// <param name="value">The new property value.</param>
        /// <param name="propertyName">The name of the property. This value is
        /// required for raising the event and must not be <c>null</c> or
        /// empty.</param>
        /// <returns><c>true</c> if the property did change, <c>false</c>
        /// otherwise.</returns>
        protected virtual bool ChangeAndNotify<T>(ref T field, T value, 
                string propertyName) {
            return this.PropertyChanged.ChangeAndNotify(ref field, value, this,
                propertyName);
        }

        /// <summary>
        /// Raises the <see cref="PropertyChanged"/> event.
        /// </summary>
        /// <param name="propertyName">The name of the property that has been 
        /// changed.</param>
        protected virtual void OnPropertyChanged(string propertyName) {
            PropertyChangedEventHandler h = this.PropertyChanged;
            if (h != null) {
                h(this, new PropertyChangedEventArgs(propertyName));
            }
        }
    }
}
