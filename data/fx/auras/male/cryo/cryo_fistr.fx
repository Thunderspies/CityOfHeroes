#############################################################
## Cryo_FistR.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	LArmLFX
		Type	PositOnly
		At	LArmR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	HandR
	End

	Event
		EName	LArmLFX
		Type	PositOnly
		At	HandR
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
	End

#########################################################

End