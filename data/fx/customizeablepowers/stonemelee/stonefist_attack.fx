#############################################################
## StoneFist_Attack.fx
#############################################################

FxInfo

Lifespan 36

Flags DontSuppress

#############################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	LARMR
		Bhvr	CustomizeablePowers\StoneMelee\stonehands.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneHandR
		Sound stonepunch 80 80 0.8
	End

	Event
		Type	Local
		At	LARML
		Bhvr	CustomizeablePowers\StoneMelee\stonehands.bhvr
		BhvrOverride
			TintGeom		1
		End
		geom	Tintable_StoneHandL
	End
End

#############################################################

Condition
	On 	Time
	Time 	30

	Event
		Type	Local
		At	LARMR
		Part	CustomizeablePowers\StoneMelee\EarthHandsDust.part
		Part	CustomizeablePowers\StoneMelee\EarthHandsDust2.part
		Part	CustomizeablePowers\StoneMelee\EarthHandsDustRocks.part
	End

	Event
		Type	Local
		At	LARML
		Part	CustomizeablePowers\StoneMelee\EarthHandsDust.part
		Part	CustomizeablePowers\StoneMelee\EarthHandsDust2.part
		Part	CustomizeablePowers\StoneMelee\EarthHandsDustRocks.part
	End
End

#############################################################

End