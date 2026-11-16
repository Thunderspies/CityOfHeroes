#############################################################
## DE_SporeCloudA.fx
#############################################################

FxInfo

########################################################

Input
	Inpname	head
End

########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	core
		Type	Local
		At	head
		Part2	:DeavouredEarthSporeCloud.part
	End


End


End
