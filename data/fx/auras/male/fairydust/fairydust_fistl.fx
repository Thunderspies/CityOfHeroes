#############################################################
## FairyDust_FistL.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	WepL
		Part	:Smoke_Stationary.part
		Part	:Dust_Stationary.part
		Part	:Dust_Stationary_Big.part
	End

	Event
		Type	PositOnly
		At	WepL
		Part	:Dust_Trail.part
		Part	:Dust_Trail_Big.part
		Part	:Smoke_Trail.part
		Part	:Dust_Stationary_Light.part
		POther	LArmL
	End

End

#########################################################

End