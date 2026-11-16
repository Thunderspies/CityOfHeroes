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
		part	:Offset_Steam_Stream.part
		part	:Offset_Steam_Stream_Lingering.part

	End
End

#########################################################

End