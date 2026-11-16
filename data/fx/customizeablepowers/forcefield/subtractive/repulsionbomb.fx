#############################################################
## RepulsionBomb.fx
#############################################################

FxInfo
Lifespan 200

################################################################################

Condition
	On 	Time
	Time 	0

	Event
		EName	Offset
		Type	PositOnly
		At	Origin
		geom	RootToChestAdjustment
		Sound repulsion 100 100 .88
	End

	Event
		Type	Start
		At	Offset
		Bhvr 	fx\CustomizeablePowers\ForceField\ForceFieldBomb.bhvr
		BhvrOverride
			Alpha			255
			TintGeom		1
		End
		Part1	:BubbleSphere8.part
		geom	Tintable_Dark_FX_ForceBubble
		Lifespan 9.5
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	Start
		At	Offset
		bhvr	behaviors\RepulsionBombRing.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Subtractive_MegaRing2
		Lifespan 7
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Start
		At	Offset
		Part1	:BubbleSphereExplosion.part
		Part2	:BubbleSphereExplosion2.part
	End
End


Condition
	On 	Time
	Time 	15

	Event
		Type	Start
		At	Offset
		Bhvr 	behaviors\powers\forcefield\ForceFieldBombBits.bhvr
		Part1	:BubbleSphere8.part
		Lifespan 6
	End
End

#############################################################

End