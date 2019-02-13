node {
	stage 'Checkout'
		checkout scm

	stage 'Build'
		bat "\"D:\\VS2017\\MSBuild\\15.0\\Bin\\msbuild.exe\" \"Team Project.sln\" /p:Configuration=Release /p:Platform=\"x64\""

	stage 'Archive'
		archive 'TeamProject/bin/Release/**'

}
