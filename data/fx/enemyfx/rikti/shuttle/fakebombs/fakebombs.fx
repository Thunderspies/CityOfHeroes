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
		EName 	OffsetPort
		At	Origin
		Type	Local 
		BhvrOverride
			PositionOffset	64 -24 16
			StartJitter	8 8 24
		End
	End

	Event
		EName 	OffsetStarboard
		At	Origin
		Type	Local 
		BhvrOverride
			PositionOffset	-64 -24 16
			StartJitter	8 8 24
		End
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	OffsetStarboard
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
		At	OffsetStarboard
		Part3	ENEMYFX\Rikti\RiktiPortalEllipsePulse.part
		Lifespan	90
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	OffsetStarboard
		Part3	ENEMYFX\Rikti\RiktiEllipsePulseShrinkAngles.part
		Lifespan	75
	End
End

#############################################################

Condition
	On	Time
	Time	0

	Event
		Type	Start
		At	OffsetPort
#		Sound 	Materialize1 500.0 100.0 .5
		Part3	ENEMYFX\Rikti\RiktiPortalSpecks.part
		Part4	ENEMYFX\Rikti\RiktiPortalStreaks.part
		Part2	ENEMYFX\Rikti\PortalRays.part
		Part3	ENEMYFX\Rikti\PortalStreakRays.part
		#Part2	ENEMYFX\Rikti\RiktiPortalRings.part
		Lifespan	90
	End

	Event
		Type	Start
		At	OffsetPort
		Part3	ENEMYFX\Rikti\RiktiPortalEllipsePulse.part
		Lifespan	90
	End
End

Condition
	On	Time
	Time	15

	Event
		Type	Start
		At	OffsetPort
		Part3	ENEMYFX\Rikti\RiktiEllipsePulseShrinkAngles.part
		Lifespan	75
	End
End

#############################################################

Condition
	On	Time
	Time	30

	Event
		EName 	BombStarboard
		At	OffsetStarboard
		Type	Start 
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			PyrRotateJitter		10 90 0
			InitialVelocityJitter	0.25 0 0.25
			Spin			0.0 0.25 0.0
			SpinJitter		0.25 0.25 0.25
			Drag			0.006
			Gravity			0.04 
			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		part	:Bomb_Trail1.part
		Geom	FX_RiktiBomb
		CEvent 	:Explosion.fx
		CThresh 0.0001
		CDestroy 1
	End

	Event
		EName 	BombStarboard_Trail1
		At	BombStarboard
		Type	Local 
		BhvrOverride
			PositionOffset	0 3 0
		End
		part1	:Bomb_Trail2.part
		part2	:Bomb_BlinkingLightGlow1.part
		part3	:Bomb_BlinkingLightCore1.part
	End

	Event
		EName 	BombStarboard_Trail2
		At	BombStarboard
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
	Time	30

	Event
		EName 	BombPort
		At	OffsetPort
		Type	Start 
		Bhvr	behaviors/fadein.bhvr
		BhvrOverride
			PyrRotateJitter		10 90 0
			InitialVelocityJitter	0.25 0 0.25
			Spin			0.0 0.25 0.0
			SpinJitter		0.25 0.25 0.25
			Drag			0.006
			Gravity			0.04 
			physics 		1
			physRadius 		0.8
			physGravity 		1
			physRestitution 	0.5
			physSFriction 	 	0.3
			physDFriction 	 	0.5
		End
		Geom	FX_RiktiBomb
		part	:Bomb_Trail1.part
		CEvent 	:Explosion.fx
		CThresh 0.0001
		CDestroy 1
	End

	Event
		EName 	BombPort_Trail1
		At	BombPort
		Type	Local 
		BhvrOverride
			PositionOffset	0 3 0
		End
		part1	:Bomb_Trail2.part
		part2	:Bomb_BlinkingLightGlow1.part
		part3	:Bomb_BlinkingLightCore1.part
	End

	Event
		EName 	BombPort_Trail2
		At	BombPort
		Type	Local 
		BhvrOverride
			PositionOffset	0 -3 0
		End
		part1	:Bomb_Trail3.part
	End
End

#############################################################

End