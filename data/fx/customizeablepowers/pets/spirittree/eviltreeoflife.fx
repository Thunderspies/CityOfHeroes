#########################################################
## Spirit Tree
#########################################################

FxInfo

#########################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	root
		Sound summon2 70 70 0.55
	End
End

Condition
	On	Time
	Time	30

	Event
		Type	Local
		At	root
		Sound spirittree_loop 70 70 0.5
	End
End

Condition
	On 	cycle
	Time 	60

	Event
		Type	local
		At	root
		Sound aura6 50 50 0.24
		Sound aura5 50 50 0.22
		Sound aura7 50 50 0.19
		LifeSpan 130
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		EName   LightRays
		Type	start
		At	Root
		part	CustomizeablePowers\Pets\SpiritTree\EvilTreeLightRays.part
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeLightRays2.part
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeLightRaysCore.part
		LifeSpan 45
	End

	Event
		EName   LightRays
		Type	start
		At	Root
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreestartupstars.part
		LifeSpan 65
	End
End

Condition
	On 	Time
	Time 	45
	Event
		EName	crack
		Type	start
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\EvilTreeSplatCracks.bhvr
		BhvrOverride
			Scale		4.0 6.0 4.0
			ScaleRate	1.0 1.0 1.0
			EndScale	8.0 8.0 8.0
			FadeInLength	60
			FadeOutLength	120
			Alpha		192
		End
		Splat	ImpactCracks.tga
	End
End

### AOE FX #######################################################

Condition
	On 	cycle
	Time 	60
	Chance	1

	Event
		Type	start
		At	Root
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTree_RingsDust.part
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTree_RingsOut.part
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTree_RingsOut2.part
		LifeSpan 30
	End
End

Condition
	On 	cycle
	Time 	60
	Chance	1

	Event
		EName   RadiusFxScalex
		Type	Start
		At	root
		Bhvr	CustomizeablePowers\Pets\SpiritTree\PlantRingScale.bhvr
		LifeSpan 50
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScalex
		bhvr    CustomizeablePowers\Pets\SpiritTree\Plantringscaler.bhvr
		Part	CustomizeablePowers\Pets\SpiritTree\AOETreeDust.part
		Part	CustomizeablePowers\Pets\SpiritTree\AOETreeDust2.part
		LifeSpan 45
	End
End

Condition
	On 	cycle
	Time 	60
	Chance	1

	Event
		EName   RadiusFxScale
		Type	Start
		At	root
		Bhvr	powers\firecontrol\FireShield.bhvr
		LifeSpan 130
	End

	Event
		EName 	Prime1
		Type	Local
		At	RadiusFxScale
		Part	CustomizeablePowers\Pets\SpiritTree\TreeDust.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeDustFlat.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeDust2Flat.part
		LifeSpan 10
	End
End

## TREE OF LIFE #######################################################

Condition
	On 	Time
	Time 	20

	Event
		Ename	TreeRoots
		Type	PositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\EvilTreeOfLifeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_TreeOfLifeEvil_rootsFullOpacity
	End

	Event
		Ename	Tree
		Type	PositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\EvilTreeOfLifeScale.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_TreeOfLifeEvilFullOpacity
	End

	Event
		Ename	TreeBranches
		Type	PositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\EvilTreeOfLifeScaleBranches.bhvr
		BhvrOverride
			TintGeom		1
		End
		Geom	Tintable_TreeOfLifeEvil_branchesFullOpacity
	End
End

Condition
	On 	Time
	Time 	60

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	-0.5 7 -1
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	3 5 -3
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	2 3 0.5
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	-0.5 4.0 3
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	0 4.5 5
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	-3 4 1.5
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	1.5 3 0.5
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	1.5 7 -0.5
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	0 6 -4
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	0 6 -5.0
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End

	Event
		Type	startPositOnly
		At	Root
		BHVR	CustomizeablePowers\Pets\SpiritTree\CanopyOffset.bhvr
		BhvrOverride
			PositionOffset	-2 6 -2
		End
		Part	CustomizeablePowers\Pets\SpiritTree\EvilTreeMoss.part
		Part	CustomizeablePowers\Pets\SpiritTree\TreeSparks.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBlack.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright.part
		Part	CustomizeablePowers\Pets\SpiritTree\GhastlyCanopyBright2.part
	End
End

###############################################################
##
##Condition
##	On 		triggerbits
##	Triggerbits	death
##
##	Event
##		Type	Local
##		At	Origin
##		Bhvr	behaviors\vanish.bhvr
##		Flags	AdoptParentEntity
##	End
##
##	Event
##		Type	Destroy
##		EName 	ALL
##	End
##
##	Event
##		Type	Start
##		At	Origin
##		ChildFX	EnemyFx\DevouringEarth\AOEPowers\PlantDeath.fx
##	End
##End
##
###############################################################

End