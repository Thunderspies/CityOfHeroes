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
		At	Chest
		geom	RootToChestAdjustment
		Sound repulsion 100 100 .88
	End

	Event
		Type	Start
		At	Offset
		Bhvr 	fx\CustomizeablePowers\ForceField\ForceFieldBomb.bhvr
		BhvrOverride
			TintGeom		1
		End
		Part1	CustomizeablePowers\ForceField\BubbleSphere8.part
		geom	Tintable_FX_ForceBubble
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
		geom	Tintable_MegaRing2
		Lifespan 7
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	Start
		At	Offset
		Part1	CustomizeablePowers\ForceField\BubbleSphereExplosion.part
		Part2	CustomizeablePowers\ForceField\BubbleSphereExplosion2.part
	End
End


Condition
	On 	Time
	Time 	15

	Event
		Type	Start
		At	Offset
		Bhvr 	behaviors\powers\forcefield\ForceFieldBombBits.bhvr
		Part1	CustomizeablePowers\ForceField\BubbleSphere8.part
		Lifespan 6
	End
End

#############################################################

End