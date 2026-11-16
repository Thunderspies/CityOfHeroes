#############################################################
## FairyDust_Hair.fx
#############################################################

FxInfo
Flags	DontSuppress

#########################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	PositOnly
		At	Head
		Part	:Smoke_Trail_Head.part
		Part	:Smoke_Stationary.part
		Part	:Dust_Stationary.part
		Part	:Dust_Stationary_Big.part
		Part	:Dust_Trail.part
		Part	:Dust_Trail_Big.part
	End
End

#########################################################

End