#############################################################
## BarbSwipe_Attack.fx
#############################################################

FxInfo

Flags InheritAlpha

LifeSpan 75

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Grow.bhvr
		BhvrOverride
			TintGeom		1
		End
		ChildFX	:Child_WepLSpine.fx
		Sound Quills2b 70.0 70.0 0.8
		Lifespan 55
	End

	Event
		Type	local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Grow.bhvr
		BhvrOverride
			TintGeom		1
		End
		ChildFX	:Child_WepRSpine.fx
		Lifespan 55
	End
End

Condition
	On 	Time
	Time 	55

	Event
		Type	Local
		At	HandL
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Shrink.bhvr
		BhvrOverride
			TintGeom		1
		End
		ChildFX	:Child_WepLSpine.fx
	End

	Event
		Type	Local
		At	HandR
		Bhvr	CustomizeablePowers\Spines\MetalSpines\LongSpikes_Shrink.bhvr
		BhvrOverride
			TintGeom		1
		End
		ChildFX	:Child_WepRSpine.fx
	End
End

## SWOOSHES ###########################################################

Condition
	On 	Time
	Time 	15

	Event
		EName	swipe
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	18

	Event
		EName	swipe01
		Type	start
		At	Chest
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	21

	Event
		EName	swipe02
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh01
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	24

	Event
		EName	swipe03
		Type	start
		At	Chest
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	27

	Event
		EName	swipe04
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh01
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	29

	Event
		EName	swipe05
		Type	start
		At	Chest
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	32

	Event
		EName	swipe06
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh01
		LifeSpan 3
	End
End

Condition
	On 	Time
	Time 	45

	Event
		EName	swipe07
		Type	start
		At	waist
		bhvr	CustomizeablePowers\Spines\Spines\Claws.bhvr
		Geom	swoosh
		LifeSpan 3
	End
End

#############################################################

End