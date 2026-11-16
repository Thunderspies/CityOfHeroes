#############################################################
## Ageless.fx
#############################################################

FxInfo
Lifespan 300
#############################################################

############################	WARM UP

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	Root
		ChildFx	IncarnatePowers/Incarnate_Gather.fx
		Lifespan 45
	End
	Event
		Type	Local
		At	Root
		Sound 	Incarnate_Ageless_01 100 100 .8
	End

End

##	Glowing hands

Condition
	On 	Time
	Time 	0

	Event
		EName	Left
		Type	Local
		At	wepL
		Part1	:HandFlare.part
		Lifespan 45
	End

	Event
		EName	Right
		Type	Local
		At	wepR
		Part1	:HandFlare.part
		Lifespan 45
	End
End


##	Ground beneath character


Condition
	On 	Time
	Time 	10

	Event
		Type	local
		At	Root
		part	:Start_Flash_01.part
		Lifespan 30
	End

	Event
		Type	local
		At	Root
		bhvrOverride
			PositionOffset 0 1.3 0
		End
		part	:Start_Flash_02.part
		part	:FlareUp_Base.part
		Lifespan 30
	End
End


##	Radiating rings


Condition
	On 	Time
	Time 	20

	Event
		EName   RadiusFxScale
		Type	local
		At	T_Root
		Bhvr	POWERS\FireControl\Inferno.bhvr
	End

	Event
		Type	PositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo15.bhvr
		bhvrOverride
			Spin 0 0.005 0
			TintGeom 1
		End
		Geom	Fx_FlameCylinderRingDark
		part	:Ring_Smoke_01.part
		Lifespan 15
	End

End

Condition
	On 	Time
	Time 	22

	Event
		Type	PositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo30.bhvr
		bhvrOverride
			Spin 0 -0.001 0
			TintGeom 1
			PositionOffset 0 0.5 0
		End
		Geom	Fx_FlameCylinderRingDark
		part	:Ring_Smoke_02.part
		Lifespan 12
	End

End

Condition
	On 	Time
	Time 	25

	Event
		Type	PositOnly
		At	RadiusFxScale
		bhvr    behaviors/Braz/ExpandRingTo60.bhvr
		bhvrOverride
			Spin 0 -0.001 0
			TintGeom 1
		End
		Geom	Fx_FlameCylinderRingDark
		part	:Ring_Smoke_02.part
		Lifespan 25
	End

End

#############################################################
End