#############################################################
## RoofFire.fx
#############################################################

FxInfo
LifeSpan 0

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Start
		At	Origin
		Part	:RoofFire_flame_A.part
		Part	:RoofFire_flame_B.part
		Sound WW_SweepingLaser_Burn_Loop 200 200 .35
	End
End



#############################################################

End