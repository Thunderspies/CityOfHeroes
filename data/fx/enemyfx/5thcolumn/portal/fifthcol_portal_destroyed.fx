#############################################################
## FifthCol_Portal_Destroyed.fx
#############################################################

FxInfo

Lifespan 300

#############################################################

#Condition
#	On 	Time
#	Time 	0
#
#	Event
#		Type	Start
#		At	Root
#		geom	_Fifthcol_Portal_Active
#		Lifespan 20
#	End
#End

#############################################################
###	CYCLING BEAMS AND GLOWS	  ###########################
#############################################################

Condition
	On	Time
	Time	0


	Event
		EName	Offsets
		Type	Start
		At	Root
		geom	GEO_5thColumnPortalOffsets
		Sound 5thColumnPortal_loop 200 200 0.25
	End

	Event
		EName	Offset
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
	End

	Event
		Type	Start
		At	Offset
		Part	:Embers_Imploding.part
		Lifespan 1
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	2
		Part	:ElectricBeamQuick_Exploding.part
		PMagnet	Offset
		Lifespan 5
	End



	Event
		Type	Start
		At	Offsets
		AltPiv	9
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 5

	End
End

Condition
	On	Time
	Time	3

	Event
		Type	Start
		At	Offsets
		AltPiv	10
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 5
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	4
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 5

	End

	Event
		Type	Start
		At	Offsets
		AltPiv	12
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10

	End


End

Condition
	On	Time
	Time	4

	Event
		Type	Start
		At	Offsets
		AltPiv	6
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	8
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	11
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10
	End


End


Condition
	On	Time
	Time	7

	Event
		Type	Start
		At	Offsets
		AltPiv	3
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	5
		Part	:ElectricBeamQuick_Exploding.part
		Part	:GlowQuick_Exploding.part
		PMagnet	Offset
		Lifespan 10

	End


End



Condition
	On 	Time
	Time 	12
	Event
		EName	Offset
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 3 0
			PyrRotate	90 90 90
		End
		Part	:Core_Glow_Exploding_Quick.part
		Part	:Sparks.part
		Lifespan 1
	End


End

Condition
	On	Time
	Time 	15

	Event
		EName	Offset
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 0 0
			PyrRotate	90 90 90
		End
		Bhvr	behaviors\CameraShake01Subtle.bhvr
		Part	:Core_Glow_Exploding.part
		Part	:Oval_Flash.part
		Part	:Oval_Exploding.part
		Part	:Oval_Tendrils_Exploding.part
		Part	:ElectricBeamQuick_Flash.part
		Part	:Embers_Exploding.part

		PMagnet	Offsets
		Lifespan 5
	End

	Event
		Type	Start
		At	Root
		Part	:Core_Glow_Exploding.part
		Part	:SmokePillar.part
		Part	:SmokePillar2.part
		Part	:SmokePillar_Additive.part
		ChildFX	:5thCol_Portal_Debris.fx
	End

	Event
		Type	Start
		At	Offsets
		AltPiv	1
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:ElectricBeamQuick_Lingering.part
		Part	:ElectricBeamQuick_Glow.part
		Lifespan 10
	End

	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset	0 1 0
		End
		Part	:Fire_Explosion.part
		Part	:FlameBase.part
		Part	:FlameAdditive.part
		Part	:BodyFire.part
		Lifespan 30
	End
End