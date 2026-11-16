#############################################################
## FairyDust_FistR.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	WepR
		Part	:Smoke_Stationary.part
		Part	:Dust_Stationary.part
		Part	:Dust_Stationary_Big.part
	End

	Event
		Type	PositOnly
		At	WepR
		Part	:Dust_Trail.part
		Part	:Dust_Trail_Big.part
		Part	:Smoke_Trail.part
		Part	:Dust_Stationary_Light.part
		POther	LArmR
	End

End

#########################################################

End