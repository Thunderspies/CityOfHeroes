#############################################################
## Cryo_Body.fx
#############################################################

FxInfo

#########################################################

Condition
	On 	Time
	Time 	0



	Event
		EName	UArmLFX
		Type	PositOnly
		At	Hair
		Part	:Mist_Cold_Detached.part
		Part	:Mist_Big.part
		Part	:Mist_Big_Trail.part
		POther	Neck
	End


#########################################################

End