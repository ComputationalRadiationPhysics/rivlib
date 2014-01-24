/// <copyright file="TheClassWizardDialog.xaml.cs" company="TheLib Team"> 
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

using System;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;


namespace The.Lib.ClassWizard {

    /// <summary>
    /// Interaction logic for TheClassWizardDialog.xaml
    /// </summary>
    public partial class TheClassWizardDialog : Window {

        #region Dependency Properties
        public static readonly DependencyProperty BaseClassNameProperty
            = DependencyProperty.Register("BaseClassName",
            typeof(string),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(null, onPropertyChanged),
            onValidateBaseClassName);

        public static readonly DependencyProperty ClassNameProperty
            = DependencyProperty.Register("ClassName",
            typeof(string),
            typeof(TheClassWizardDialog),
            new PropertyMetadata("the::class", onPropertyChanged),
            onValidateClassName);

        public static readonly DependencyProperty HeaderFileProperty
            = DependencyProperty.Register("HeaderFile",
            typeof(string),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(null, onPropertyChanged, onCoerceHeaderFile),
            onValidatePath);

        public static readonly DependencyProperty IsDestructorVirtualProperty
            = DependencyProperty.Register("IsDestructorVirtual",
            typeof(bool),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(true, onPropertyChanged));

        public static readonly DependencyProperty IsMakeCopyableProperty
            = DependencyProperty.Register("IsMakeCopyable",
            typeof(bool),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(false, onPropertyChanged));

        public static readonly DependencyProperty IsMakeNotCopyableProperty
            = DependencyProperty.Register("IsMakeNotCopyable",
            typeof(bool),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(false, onPropertyChanged));

        public static readonly DependencyProperty IsMakeNotInstantiableProperty
            = DependencyProperty.Register("IsMakeNotInstantiable",
            typeof(bool),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(false, onPropertyChanged));

        public static readonly DependencyProperty SourceFileProperty
            = DependencyProperty.Register("SourceFile",
            typeof(string),
            typeof(TheClassWizardDialog),
            new PropertyMetadata(string.Empty, onPropertyChanged, onCoerceSourceFile),
            onValidatePath);

        public static readonly DependencyProperty TemplateParameterListProperty 
            = DependencyProperty.Register("TemplateParameterList", 
            typeof(string), 
            typeof(TheClassWizardDialog),
            new PropertyMetadata(null, onPropertyChanged),
            onValidateTemplateParameterList);
        #endregion

        /// <summary>
        /// Initialise a new instance.
        /// </summary>
        public TheClassWizardDialog() {
            this.InitializeComponent();
            this.DataContext = this;
            this.CoerceValue(HeaderFileProperty);
            this.CoerceValue(SourceFileProperty);
        }

        #region CLR Properties
        /// <summary>
        /// Getr or sets the fully qualified name of the base class.
        /// </summary>
        public string BaseClassName {
            get {
                return (string) this.GetValue(BaseClassNameProperty);
            }
            set {
                this.SetValue(BaseClassNameProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets the fully qualified name of the class to be generated.
        /// </summary>
        public string ClassName {
            get {
                return (string) this.GetValue(ClassNameProperty);
            }
            set {
                this.SetValue(ClassNameProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets the path of the header file to be generated.
        /// </summary>
        public string HeaderFile {
            get {
                return (string) this.GetValue(HeaderFileProperty);
            }
            set {
                this.SetValue(HeaderFileProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets whether the destructor that is generated should be
        /// virtual.
        /// </summary>
        public bool IsDestructorVirtual {
            get {
                return (bool) this.GetValue(IsDestructorVirtualProperty);
            }
            set { this.SetValue(IsDestructorVirtualProperty, value); }
        }

        /// <summary>
        /// Gets or sets whether the class that is generated should have a
        /// copy constructor and an assignment operator.
        /// </summary>
        public bool IsMakeCopyable {
            get {
                return (bool) this.GetValue(IsMakeCopyableProperty);
            }
            set {
                this.SetValue(IsMakeCopyableProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets whether the class that is generated should derive from
        /// <c>the::not_copyable</c>.
        /// </summary>
        public bool IsMakeNotCopyable {
            get {
                return (bool) this.GetValue(IsMakeNotCopyableProperty);
            }
            set { this.SetValue(IsMakeNotCopyableProperty, value); }
        }

        /// <summary>
        /// Gets or sets whether the class that is generated should derive from
        /// <c>the::not_instantiable</c>.
        /// </summary>
        public bool IsMakeNotInstantiable {
            get {
                return (bool) this.GetValue(IsMakeNotInstantiableProperty);
            }
            set { this.SetValue(IsMakeNotInstantiableProperty, value); }
        }

        /// <summary>
        /// Gets or sets the path of the source file to be generated.
        /// </summary>
        public string SourceFile {
            get {
                return (string) this.GetValue(SourceFileProperty);
            }
            set {
                this.SetValue(SourceFileProperty, value);
            }
        }

        /// <summary>
        /// Gets or sets the template parameter list.
        /// </summary>
        public string TemplateParameterList {
            get { 
                return (string) this.GetValue(TemplateParameterListProperty);
            }
            set { 
                this.SetValue(TemplateParameterListProperty, value); 
            }
        }
        #endregion

        private static string[] getClassNameParts(DependencyObject d) {
            string name = (string) d.GetValue(ClassNameProperty);
            return name.Split(NAMESPACE_DELIMITER, 
                StringSplitOptions.RemoveEmptyEntries);
        }

        #region Dependency Property Event Handlers
        /// <summary>
        /// Computes the path to the header file from the class name if the
        /// <paramref name="baseValue"/> is empty.
        /// </summary>
        /// <param name="d"></param>
        /// <param name="baseValue"></param>
        /// <returns></returns>
        private static object onCoerceHeaderFile(DependencyObject d, 
                object baseValue) {
            string value = baseValue as string;

            if (string.IsNullOrWhiteSpace(value)) {
                string[] nameParts = TheClassWizardDialog.getClassNameParts(d);
                StringBuilder sb = new StringBuilder(
                    Properties.Resources.DefaultHeaderDirectory);
                sb.Append(DIRECTORY_SEPARATOR);
                sb.Append(string.Join(DIRECTORY_SEPARATOR, nameParts));
                sb.Append(Properties.Resources.DefaultHeaderExtension);
                return sb.ToString();
            } else {
                return value;
            }
        }

        /// <summary>
        /// Computes the path to the source file from the class name if the
        /// <paramref name="baseValue"/> is empty.
        /// </summary>
        /// <param name="d"></param>
        /// <param name="baseValue"></param>
        /// <returns></returns>
        private static object onCoerceSourceFile(DependencyObject d, 
                object baseValue) {
            string value = baseValue as string;

            if (string.IsNullOrWhiteSpace(value)) {
                string[] nameParts = TheClassWizardDialog.getClassNameParts(d);
                StringBuilder sb = new StringBuilder(
                    Properties.Resources.DefaultSourceDirectory);
                sb.Append(DIRECTORY_SEPARATOR);
                sb.Append(string.Join(DIRECTORY_SEPARATOR, nameParts.Skip(1)));
                sb.Append(Properties.Resources.DefaultSourceExtension);
                return sb.ToString();
            } else {
                return value;
            }
        }

        private static void onPropertyChanged(DependencyObject d,
                DependencyPropertyChangedEventArgs e) {
            TheClassWizardDialog dlg = (TheClassWizardDialog) d;

            if (e.Property == ClassNameProperty) {
                d.CoerceValue(HeaderFileProperty);
                d.CoerceValue(SourceFileProperty);

            } else if (e.Property == IsMakeCopyableProperty) {
                if ((bool) e.NewValue) {
                    // If the last decision of the user was to make the new 
                    // class copyable, it must not derive from 'not_copyable'
                    // and it must be instantiable. Any other combination would
                    // not make sense.
                    dlg.IsMakeNotCopyable = false;
                    dlg.IsMakeNotInstantiable = false;
                }

            } else if (e.Property == IsMakeNotCopyableProperty) {
                if ((bool) e.NewValue) {
                    // If the last decision of the user was to forbid copies of 
                    // the new class, the dtor must be virtual, because we
                    // forbid copies via inheritance from a class with private 
                    // ctor. Furthermore, it does not make any sense to create 
                    // a copy ctor and assigment and to forbid instances.
                    dlg.IsDestructorVirtual = true;
                    dlg.IsMakeCopyable = false;
                    dlg.IsMakeNotInstantiable = false;
                }

            } else if (e.Property == IsMakeNotInstantiableProperty) {
                if ((bool) e.NewValue) {
                    // If the last decision of the user was to disallow instances,
                    // the dtor must be virtual, because we forbid instances via
                    // inheritance from a class with private ctor and dtor. 
                    // Furthermore, it does not make sense to create a copy ctor 
                    // and an assignment operator for classes that cannot have
                    // instances. Last but not least, disallowing instances 
                    // implies disallowing copies, because the not_instantiable
                    // base class inherits from not_copyable.
                    dlg.IsDestructorVirtual = true;
                    dlg.IsMakeCopyable = false;
                    dlg.IsMakeNotCopyable = false;
                }
            }

            // Check whether we can enable the "OK" button:
            //dlg.btnOk.IsEnabled = !dlg.hasValidationError();
        }

        /// <summary>
        /// Validates the base class name, which can be empty or valid.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private static bool onValidateBaseClassName(object value) {
            string v = value as string;
            return (!string.IsNullOrWhiteSpace(v)) ? onValidateClassName(value)
                                                   : true;
        }

        /// <summary>
        /// Validates a class name, which must not be empty.
        /// </summary>
        /// <param name="value"></param>
        /// <returns></returns>
        private static bool onValidateClassName(object value) {
            string fqn = value as string;

            if (fqn == null) {
                // Input is not a string or empty, which is wrong.
                return false;
            }

            string[] parts = fqn.Split(NAMESPACE_DELIMITER,
                StringSplitOptions.None);

            foreach (string p in parts) {
                if (string.IsNullOrWhiteSpace(p)) {
                    // There is an empty namespace or no class name.
                    return false;
                }
                foreach (char n in NAMESPACE_DELIMITER.Single()) {
                    if (p.Contains(n)) {
                        // There is a partial namespace delimiter in the name.
                        return false;
                    }
                }
            }

            // No problem found at this point.
            return true;
        }

        private static bool onValidatePath(object value) {
            string path = value as string;

            if (string.IsNullOrWhiteSpace(path)) {
                return true;
            }

            foreach (char c in System.IO.Path.GetInvalidPathChars()) {
                if (path.Contains(c)) {
                    return false;
                }
            }

            return true;
        }

        private static bool onValidateTemplateParameterList(object value) {
            string tpl = value as string;

            if (!string.IsNullOrWhiteSpace(tpl)) {
                string[] ps = tpl.Split(',');

                foreach (string p in ps) {
                    string[] ds = p.Split((char[]) null, 
                        StringSplitOptions.RemoveEmptyEntries);
                    if (ds.Length != 2) {
                        // Must have class/typename/... and name.
                        return false;
                    }
                    if (!VALID_TEMPLATE_TYPES.Contains(ds[0].Trim())) {
                        // First must be class/typename/...
                        return false;
                    }
                    if (!char.IsLetter(ds[1][0]) && (ds[1][0] == '_')) {
                        // Name must be valid identifier.
                        return false;
                    }
                }
            }
            /* No error found at this point. */

            return true;
        }
        #endregion

        /// <summary>
        /// Gets the directory separator character constant from
        /// <see cref="System.IO.Path"/> as a string.
        /// </summary>
        /// <remarks>
        /// <para>This cannot be a static constant because of the initialisation
        /// order of constants and dependency properties. Using a property is
        /// safe under any condition and performance does not matter for the
        /// class wizard.</para>
        /// </remarks>
        private static string DIRECTORY_SEPARATOR  {
            get {
                return System.IO.Path.DirectorySeparatorChar.ToString();
            }
        }

        /// <summary>
        /// Gets the C++ namespace delimiter for splitting the class name.
        /// </summary>
        /// <remarks>
        /// <para>This cannot be a static constant because of the initialisation
        /// order of constants and dependency properties. Using a property is
        /// safe under any condition and performance does not matter for the
        /// class wizard.</para>
        /// <para>The property is an array for begin directly used as parameter
        /// of <see cref="System.String.Split"/>.</para>
        /// </remarks>
        private static string[] NAMESPACE_DELIMITER {
            get {
                return new string[] { "::" };
            }
        }

        private static string[] VALID_TEMPLATE_TYPES {
            get {
                return new string[] { "class", "typename", "int" };
            }
        }

        private bool hasValidationError(DependencyObject obj = null) {
            if (obj == null) {
                obj = this;
            }

            if (Validation.GetHasError(obj)) {
                return true;
            } else {
                var children = LogicalTreeHelper.GetChildren(
                    obj).OfType<DependencyObject>();
                return children.Any(c => this.hasValidationError(c));
            }
        }

        #region Event Handlers
        private void onBtnOkClick(object sender, RoutedEventArgs e) {
            if (!this.hasValidationError()) {
                this.DialogResult = true;
                this.Close();
            }
        }

        private void onBtnCancelClick(object sender, RoutedEventArgs e) {
            this.DialogResult = false;
            this.Close();
        }
        #endregion
    }
}
