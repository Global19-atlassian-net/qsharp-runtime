// <auto-generated>
// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License. See License.txt in the project root for
// license information.
//
// Code generated by Microsoft (R) AutoRest Code Generator.
// Changes may cause incorrect behavior and will be lost if the code is
// regenerated.
// </auto-generated>

namespace Microsoft.Azure.Quantum.Client.Models
{
    using Microsoft.Rest;
    using Newtonsoft.Json;
    using System.Collections;
    using System.Collections.Generic;
    using System.Linq;

    /// <summary>
    /// Job details.
    /// </summary>
    public partial class JobDetails
    {
        /// <summary>
        /// Initializes a new instance of the JobDetails class.
        /// </summary>
        public JobDetails()
        {
            CustomInit();
        }

        /// <summary>
        /// Initializes a new instance of the JobDetails class.
        /// </summary>
        /// <param name="containerUri">The blob container SAS uri, the
        /// container is used to host job data.</param>
        /// <param name="inputDataFormat">The format of the input data.</param>
        /// <param name="providerId">The unique identifier for the
        /// provider.</param>
        /// <param name="target">The target identifier to run the job.</param>
        /// <param name="id">The job id.</param>
        /// <param name="name">The job name. Is not required for the name to be
        /// unique and it's only used for display purposes.</param>
        /// <param name="inputDataUri">The input blob SAS uri, if specified, it
        /// will override the default input blob in the container.</param>
        /// <param name="inputParams">The input parameters for the job. JSON
        /// object used by the target solver. It is expected that the size of
        /// this object is small and only used to specify parameters for the
        /// execution target, not the input data.</param>
        /// <param name="metadata">The job metadata. Metadata provides client
        /// the ability to store client-specific information</param>
        /// <param name="outputDataUri">The output blob SAS uri. When a job
        /// finishes successfully, results will be uploaded to this
        /// blob.</param>
        /// <param name="outputDataFormat">The format of the output
        /// data.</param>
        /// <param name="status">The job status. Possible values include:
        /// 'Waiting', 'Executing', 'Succeeded', 'Failed', 'Cancelled'</param>
        /// <param name="creationTime">The creation time of the job.</param>
        /// <param name="beginExecutionTime">The time when the job began
        /// execution.</param>
        /// <param name="endExecutionTime">The time when the job finished
        /// execution.</param>
        /// <param name="cancellationTime">The time when a job was successfully
        /// cancelled.</param>
        /// <param name="errorData">The error data for the job. This is
        /// expected only when Status 'Failed'.</param>
        public JobDetails(string containerUri, string inputDataFormat, string providerId, string target, string id = default(string), string name = default(string), string inputDataUri = default(string), object inputParams = default(object), IDictionary<string, string> metadata = default(IDictionary<string, string>), string outputDataUri = default(string), string outputDataFormat = default(string), string status = default(string), System.DateTime? creationTime = default(System.DateTime?), System.DateTime? beginExecutionTime = default(System.DateTime?), System.DateTime? endExecutionTime = default(System.DateTime?), System.DateTime? cancellationTime = default(System.DateTime?), ErrorData errorData = default(ErrorData))
        {
            Id = id;
            Name = name;
            ContainerUri = containerUri;
            InputDataUri = inputDataUri;
            InputDataFormat = inputDataFormat;
            InputParams = inputParams;
            ProviderId = providerId;
            Target = target;
            Metadata = metadata;
            OutputDataUri = outputDataUri;
            OutputDataFormat = outputDataFormat;
            Status = status;
            CreationTime = creationTime;
            BeginExecutionTime = beginExecutionTime;
            EndExecutionTime = endExecutionTime;
            CancellationTime = cancellationTime;
            ErrorData = errorData;
            CustomInit();
        }

        /// <summary>
        /// An initialization method that performs custom operations like setting defaults
        /// </summary>
        partial void CustomInit();

        /// <summary>
        /// Gets or sets the job id.
        /// </summary>
        [JsonProperty(PropertyName = "id")]
        public string Id { get; set; }

        /// <summary>
        /// Gets or sets the job name. Is not required for the name to be
        /// unique and it's only used for display purposes.
        /// </summary>
        [JsonProperty(PropertyName = "name")]
        public string Name { get; set; }

        /// <summary>
        /// Gets or sets the blob container SAS uri, the container is used to
        /// host job data.
        /// </summary>
        [JsonProperty(PropertyName = "containerUri")]
        public string ContainerUri { get; set; }

        /// <summary>
        /// Gets or sets the input blob SAS uri, if specified, it will override
        /// the default input blob in the container.
        /// </summary>
        [JsonProperty(PropertyName = "inputDataUri")]
        public string InputDataUri { get; set; }

        /// <summary>
        /// Gets or sets the format of the input data.
        /// </summary>
        [JsonProperty(PropertyName = "inputDataFormat")]
        public string InputDataFormat { get; set; }

        /// <summary>
        /// Gets or sets the input parameters for the job. JSON object used by
        /// the target solver. It is expected that the size of this object is
        /// small and only used to specify parameters for the execution target,
        /// not the input data.
        /// </summary>
        [JsonProperty(PropertyName = "inputParams")]
        public object InputParams { get; set; }

        /// <summary>
        /// Gets or sets the unique identifier for the provider.
        /// </summary>
        [JsonProperty(PropertyName = "providerId")]
        public string ProviderId { get; set; }

        /// <summary>
        /// Gets or sets the target identifier to run the job.
        /// </summary>
        [JsonProperty(PropertyName = "target")]
        public string Target { get; set; }

        /// <summary>
        /// Gets or sets the job metadata. Metadata provides client the ability
        /// to store client-specific information
        /// </summary>
        [JsonProperty(PropertyName = "metadata")]
        public IDictionary<string, string> Metadata { get; set; }

        /// <summary>
        /// Gets or sets the output blob SAS uri. When a job finishes
        /// successfully, results will be uploaded to this blob.
        /// </summary>
        [JsonProperty(PropertyName = "outputDataUri")]
        public string OutputDataUri { get; set; }

        /// <summary>
        /// Gets or sets the format of the output data.
        /// </summary>
        [JsonProperty(PropertyName = "outputDataFormat")]
        public string OutputDataFormat { get; set; }

        /// <summary>
        /// Gets the job status. Possible values include: 'Waiting',
        /// 'Executing', 'Succeeded', 'Failed', 'Cancelled'
        /// </summary>
        [JsonProperty(PropertyName = "status")]
        public string Status { get; private set; }

        /// <summary>
        /// Gets the creation time of the job.
        /// </summary>
        [JsonProperty(PropertyName = "creationTime")]
        public System.DateTime? CreationTime { get; private set; }

        /// <summary>
        /// Gets the time when the job began execution.
        /// </summary>
        [JsonProperty(PropertyName = "beginExecutionTime")]
        public System.DateTime? BeginExecutionTime { get; private set; }

        /// <summary>
        /// Gets the time when the job finished execution.
        /// </summary>
        [JsonProperty(PropertyName = "endExecutionTime")]
        public System.DateTime? EndExecutionTime { get; private set; }

        /// <summary>
        /// Gets the time when a job was successfully cancelled.
        /// </summary>
        [JsonProperty(PropertyName = "cancellationTime")]
        public System.DateTime? CancellationTime { get; private set; }

        /// <summary>
        /// Gets the error data for the job. This is expected only when Status
        /// 'Failed'.
        /// </summary>
        [JsonProperty(PropertyName = "errorData")]
        public ErrorData ErrorData { get; private set; }

        /// <summary>
        /// Validate the object.
        /// </summary>
        /// <exception cref="ValidationException">
        /// Thrown if validation fails
        /// </exception>
        public virtual void Validate()
        {
            if (ContainerUri == null)
            {
                throw new ValidationException(ValidationRules.CannotBeNull, "ContainerUri");
            }
            if (InputDataFormat == null)
            {
                throw new ValidationException(ValidationRules.CannotBeNull, "InputDataFormat");
            }
            if (ProviderId == null)
            {
                throw new ValidationException(ValidationRules.CannotBeNull, "ProviderId");
            }
            if (Target == null)
            {
                throw new ValidationException(ValidationRules.CannotBeNull, "Target");
            }
        }
    }
}
