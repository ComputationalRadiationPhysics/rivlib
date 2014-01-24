/// <copyright file="TheClassWizard.cs" company="TheLib Team"> 
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
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using EnvDTE;
using EnvDTE80;
using Microsoft.VisualStudio.Shell;
using Microsoft.VisualStudio.TextTemplating;
using Microsoft.VisualStudio.TextTemplating.VSHost;
using Microsoft.VisualStudio.VCProjectEngine;
using The.Lib.ClassWizard.Templates;


namespace The.Lib.ClassWizard {

    /// <summary>
    /// Implements the class wizard for creating new TheLib classes following
    /// the style guide.
    /// </summary>
    [ComVisible(true)]
    [Guid("20184B81-7C38-4E02-A1E3-8D564EEC2D25"), 
        ProgId("The.Lib.ClassWizard.TheClassWizard")]
    public class TheClassWizard : IDTWizard {

        public void Execute(object application, int hwndOwner, 
                ref object[] contextParams, ref object[] customParams, 
                ref wizardResult retval) {
            DTE2 app = (DTE2) application;
            Solution2 solution = (Solution2) app.Solution;

            // The third item in ContextParams is the pProjectIitems object.
            ProjectItems pi = (ProjectItems) contextParams[2];
            Project cp = pi.ContainingProject;

            //cp.ProjectItems.AddFromFile();
            //cp.ProjectItems.AddFromTemplate();

            //// Get the project from the ProjectItems reference.
            //project = pi.ContainingProject;
   
            //// We use the CodeModel to find out which language is in use.
            //CodeModel cm = project.CodeModel;


            TheClassWizardDialog dlg = new TheClassWizardDialog();
            dlg.ClassName = "the::" + Path.GetFileNameWithoutExtension((string) contextParams[4]);
            if (dlg.ShowDialog() == true) {
                // TODO: UI validation

                Header headerTransform = new Header();
                headerTransform.Session = this.createT4Session(dlg);
                headerTransform.Session["FileName"] = dlg.HeaderFile;
                headerTransform.Initialize();
                string headerContent = headerTransform.TransformText();

                Source sourceTransform = new Source();
                sourceTransform.Session = headerTransform.Session;
                sourceTransform.Session["FileName"] = dlg.SourceFile;
                sourceTransform.Initialize();
                string sourceContent = sourceTransform.TransformText();

                // TODO: error checking
                string projectDir = Path.GetDirectoryName(cp.FileName);
                string headerPath = Path.Combine(projectDir, dlg.HeaderFile);
                this.ensureContainingDirectory(headerPath);
                File.WriteAllText(headerPath, headerContent);
                pi.AddFromFile(headerPath);

                string sourcePath = Path.Combine(projectDir, dlg.SourceFile);
                this.ensureContainingDirectory(sourcePath);
                File.WriteAllText(sourcePath, sourceContent);
                pi.AddFromFile(sourcePath);
                // TODO: fix filters

                retval = wizardResult.wizardResultSuccess;
            } else {
                retval = wizardResult.wizardResultCancel;
            }
        }

        /// <summary>
        /// Initialises a T4 session from the parameters specified in the wizard
        /// dialog <paramref name="dlg"/>.
        /// </summary>
        /// <param name="dlg"></param>
        /// <returns></returns>
        private ITextTemplatingSession createT4Session(
                TheClassWizardDialog dlg) {
            ServiceProvider svcProv = ServiceProvider.GlobalProvider;
            ITextTemplatingSessionHost t4Host = svcProv.GetService(
                typeof(STextTemplating)) as ITextTemplatingSessionHost;
            ITextTemplatingSession retval = t4Host.CreateSession();

            retval["FileName"] = string.Empty;
            retval["FqClassName"] = dlg.ClassName;
            retval["BaseClassName"] = dlg.BaseClassName ?? string.Empty;
            retval["MakeDtorVirtual"] = dlg.IsDestructorVirtual;
            retval["MakeCopyable"] = dlg.IsMakeCopyable;
            retval["MakeNotCopyable"] = dlg.IsMakeNotCopyable;
            retval["MakeNotInstantiable"] = dlg.IsMakeNotInstantiable;
            retval["TemplateParams"] = dlg.TemplateParameterList ?? string.Empty;

            return retval;
        }

        /// <summary>
        /// Checks whether the directory that contains <paramref name="path"/> 
        /// exists and creates it if this is not the case.
        /// </summary>
        /// <param name="path"></param>
        private void ensureContainingDirectory(string path) {
            string dir = Path.GetDirectoryName(path);
            if (!Directory.Exists(dir)) {
                Directory.CreateDirectory(dir);
            }
        }
    }
}
