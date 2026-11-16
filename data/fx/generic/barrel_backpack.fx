
FxInfo

#####  barrel prop   #######################################

Condition
	On 	Time  
	Time 	0

	Event
		EName	Torch
		Type	Local
		At	Back
		Geom	GEO_small_barrel
		bhvr	behaviors/barrel_backpack.bhvr
	End
End

########  Fuse  #########################

Condition
	On 	Time  
	Time 	0

	Event
		EName 	P
		Type	local
		At	Back
		Bhvr	Behaviors/fuse_offset.bhvr	
		Part	POWERS\ItemsAndTempPowers\tiny_fire.part
		Part	POWERS\ItemsAndTempPowers\tiny_fire.part
		Part	POWERS\ItemsAndTempPowers\tiny_fire.part
		Part	POWERS\ItemsAndTempPowers\tiny_fire.part
		Part	POWERS\ItemsAndTempPowers\Sparks.part
#		Part	POWERS\ItemsAndTempPowers\Smoke.part
		
	End

End

#####  fuse fx #############################################








End		