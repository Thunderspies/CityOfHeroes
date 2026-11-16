#########################################################
##	iceweapon
########################################################
FxInfo

Condition
	On 	Time
	Time 	0

	Event
		EName 	Prime8
		Type	local
		At	Origin
		
			#BHVR	Behaviors/OffsetChest.bhvr
		Part	:Steam.part
		Part	:Flame.part	
		
		Part    :Flame2MovementOnly.part
		Part    :Lava_mist.part
		Part	:Lava_MistMoving.part
		Part    :LavaRing.part
		Part	:LavaTrail.part
		Part	:LavaRingSolid.part
		sound	lavaburn_loop 40 40 .84
		
	End

	Event
		EName 	Prime8
		Type	posit
		At	Origin
		Part	:LavaRingSolid.part
		Part	:sparks.part
	End

End


End
