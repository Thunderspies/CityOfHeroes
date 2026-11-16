#############################################################
## Android_Plasma_ChargedBlast.fx
#############################################################

FxInfo
LifeSpan 180

#############################################################

Input
	InpName Hips
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
	#	sound Plasma_Welding_Ray 200 200 .8
	End

	Event
		Ename	Flash
		Type	Local
		At	WepR #C_Neck
		Part	:Idle_SprayPainting_Core.part
		Part	:Idle_SprayPainting_Droplets.part
	End
End

#############################################################


#############################################################

End