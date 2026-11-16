#############################################################
## Entangle_Continuing.FX
#############################################################

FxInfo

ClampMinScale 0.75 0.75 0.75
ClampMaxScale 1.25 1.25 1.25

#########################################################

Input
	Inpname	Hips
End

#########################################################

Condition
	On 	Time
	Time	0

	Event
		Ename	vines
		Type	posit
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		Part	CustomizeablePowers\PlantControl\VineDust.part
		Part	CustomizeablePowers\PlantControl\VineDust2.part
		Sound Tuatha_loop 60 60 0.66
	End

	Event
		Ename	Hookup
		Type	Local
		At	root
		Bhvr	CustomizeablePowers\PlantControl\spin.bhvr
		geom	Centerdummy
	End
End

#############################################################

Condition
	On 	Time
	Time	1

	Event
		Type	start
		At	FootR
		Bhvr	CustomizeablePowers\PlantControl\VinespinLeg.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
	End

	Event
		Type	start
		At	FootR
		Bhvr	CustomizeablePowers\PlantControl\VinespinLegCounter.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig
	End

	Event
		Type	start
		At	FootL
		Bhvr	CustomizeablePowers\PlantControl\VinespinLeg.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
	End

	Event
		Type	start
		At	FootL
		Bhvr	CustomizeablePowers\PlantControl\VinespinLegCounter.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig
	End
End

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	1

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	2

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	3

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	4

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	5

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	6

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	7

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	8

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	9

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

Condition
	On 	Time
	Time 	10

	Event
		Type	start
		At	Hookup
		AltPiv	1
		Bhvr	CustomizeablePowers\PlantControl\VineFade.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_straightvine2
		geom	Tintable_straightvine
	End
End

#############################################################

Condition
	On 	Time
	Time	1

	Event
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespin.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
	End

	Event
		Ename	vines
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCounter.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig
	End

	Event
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespina.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig
	End

	Event
		Ename	vines
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCountera.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
		geom	Tintable_vinebig
	End

	Event
		Ename	vines
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\VinespinCountera.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
	End

	Event
		Ename	vines
		Type	start
		At	root
		Bhvr	CustomizeablePowers\PlantControl\Vinespina.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_vinebig1
	End
End

#############################################################

End