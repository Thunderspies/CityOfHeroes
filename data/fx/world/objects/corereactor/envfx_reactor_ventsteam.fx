#############################################################
## EnvFX_CoreReactor.fx
#############################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		part	:Offset_Steam.part
		part	:Offset_Steam_Lingering.part

	End
End

#########################################################

End