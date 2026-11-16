#############################################################
## EnvFX_SubwayDebris.fx
#############################################################

FxInfo

#############################################################


###		SHAKE
Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Root
		Sound 	Earthquake_Heavy_01 200 200 1
		Sound 	Earthquake_Heavy_02 200 200 1
		Sound 	Earthquake_Heavy_03 200 200 1
	End

	Event
		Type	Start
		At	Root
		Bhvr	behaviors\CameraShake_Subway_Subtle.bhvr
	End
End

Condition
	On	Time
	Time	10
	Event
		Type	Start
		At	Root
		Bhvr	behaviors\CameraShake_Subway_Subtle.bhvr
	End
End

####		FALLING DUST

Condition
	On	Time
	Time	0
	Repeat	1

	Event
		EName	CoreAnchor
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 5 0
			StartJitter		10 5 5
		End
		Part	:Offset_CementDust_Stream.part
		Part	:Offset_ConcreteDust_Stream.part
		Lifespan 50
	End
End

Condition
	On	Time
	Time	10
	Event
		Type	Start
		At	Root
		BhvrOverride
			PositionOffset		0 5 0
			StartJitter		10 5 5
		End
		Part	:Offset_CementDust_Stream.part
		Part	:Offset_ConcreteDust_Stream.part
		Lifespan 50
	End
End

Condition
	On	Time
	Time	0

	Event
		EName	DebrisAnchor
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset		0 -10 0
		End
	End
End

####		FALLING CONCRETE DEBRIS

Condition
	On	Time
	Time 	35
	Chance	0.5
	Repeat	4

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	DebrisAnchor
		Geom	Rubble_ConcreteSlab_01
		Geom	Rubble_ConcreteSlab_02
		Geom	Rubble_ConcreteSlab_03

		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0 20 0
			StartJitter		10 10 5
			Scale			0.02 0.015 0.02
			InitialVelocity		0.0 -1 0.0
			InitialVelocityJitter	0.2 0.6 0.2
			Spin			0.0 0.0 0.0
			SpinJitter		0.05 0.05 0.05
			StartColor		160 167 169
			PyrRotateJitter		180 180 180
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9
		End
		Geom	Earth_FXDebris01A
		CEvent	:CHILD_RockBreak.fx
		Lifespan 500
		LifespanJitter	90
	End
End

Condition
	On 	Time
	Time 	300

	Event
		EName 	All
		Type	Destroy
	End


End

#########################################################

End