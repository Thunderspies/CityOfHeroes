#############################################################
## FX System Name
#############################################################

FxInfo

Lifespan 300

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName 	Offset
		At	Origin
		Type	Local 
		BhvrOverride
			PositionOffset	-32 -24 16
			StartJitter	24 8 64
		End
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	Offset
		Sound Fakebomb1 500 500 .5
		Part3	ENEMYFX\Rikti\RiktiPortalSpecks.part
		Part4	ENEMYFX\Rikti\RiktiPortalStreaks.part
		Part2	ENEMYFX\Rikti\PortalRays.part
		Part3	ENEMYFX\Rikti\PortalStreakRays.part
		#Part2	ENEMYFX\Rikti\RiktiPortalRings.part
		Lifespan	90
	End

	Event
		Type	Start
		At	Offset
		Part3	ENEMYFX\Rikti\RiktiPortalEllipsePulse.part
		Lifespan	90
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	Offset
		Part3	ENEMYFX\Rikti\RiktiEllipsePulseShrinkAngles.part
		Lifespan	75
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	Bomb
		At	Offset
		Type	Start 
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			PyrRotateJitter		10 90 0
			InitialVelocityJitter	0.25 0 0.25
			Spin			0.0 0.25 0.0
			SpinJitter		0.25 0.25 0.25
			Drag			0.006
			Gravity			0.04 
		End
		part	:Bomb_Trail1.part
		Geom	FX_RiktiBomb
	End

	Event
		EName 	Bomb_Trail1
		At	Bomb
		Type	Local 
		BhvrOverride
			PositionOffset	0 3 0
		End
		part1	:Bomb_Trail2.part
		part2	:Bomb_BlinkingLightGlow1.part
		part3	:Bomb_BlinkingLightCore1.part
	End

	Event
		EName 	Bomb_Trail2
		At	Bomb
		Type	Local 
		BhvrOverride
			PositionOffset	0 -3 0
		End
		part1	:Bomb_Trail3.part
	End
End

#############################################################

Condition
	On	Time
	Time	115

	Event
		EName 	Explosion
		At	Bomb
		Type	Start
		ChildFX	:Explosion.fx
	End

	Event
		EName 	Bomb
		Type	Destroy
	End

	Event
		EName 	Bomb_Trail1
		Type	Destroy
	End

	Event
		EName 	Bomb_Trail2
		Type	Destroy
	End
End