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
		Bhvr	behaviors\CameraShake_Subtle60.bhvr
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
	Time 	40
	Chance	0.9
	Repeat	3

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
			Scale			0.04 0.025 0.04
			InitialVelocity		0.0 -2 0.0
			InitialVelocityJitter	0.2 0.9 0.2
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
		ChildFX :DebrisMetal_SFX.fx
		CEvent	:CHILD_RockBreak.fx
		CEvent	:DebrisConcrete_SFX.fx
		Lifespan 500
		LifespanJitter	90
	End
End

Condition
	On	Time
	Time 	50
	Chance	0.6
	Repeat	3

	Event
		SoftwareOnly
		Ename	parts2
		Type	Start
		At	DebrisAnchor
		Geom	Rubble_ConcreteSlab_01
		Geom	Rubble_ConcreteSlab_02
		Geom	Rubble_ConcreteSlab_03
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0 20 0
			StartJitter		10 10 5
			Scale			0.08 0.05 0.08
			InitialVelocity		0.0 -02 0.0
			InitialVelocityJitter	0.2 1.2 0.2
			Spin			0.0 0.0 0.0
			SpinJitter		0.05 0.05 0.05
			StartColor		160 167 169
			PyrRotateJitter		180 180 180
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.2
			physSFriction 	 	5
			physDFriction 	 	0.9
		End
		Geom	Earth_FXDebris01A
		CEvent	:CHILD_RockBreak.fx
		#CEvent	:DebrisConcrete_SFX.fx
		Lifespan 500
		LifespanJitter	90
	End
End


####		FALLING METAL DEBRIS

Condition
	On	Time
	Time 	30
	Chance	7
	Repeat	2

	Event
		SoftwareOnly
		Ename	parts3
		Type	StartPositOnly
		At	DebrisAnchor
		Geom	Widget_26_Alluminum
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride

			PositionOffset		0 20 0
			StartJitter		10 10 5
			StartColor		230 230 250
			PyrRotateJitter		180 180 180
			Scale			0.4 0.4 0.4
			Spin			0.0 0.0 0.0
			SpinJitter		0.2 0.2 0.2
			InitialVelocity		0.0 -0.8 0.0
			InitialVelocityJitter	0.3 0.6 0.3
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9

		End
		CEvent	:MetalDebrisSparks.fx		#V_COV\PhysicsEnabled\ScrapeSparks.fx
							#CEvent	:DebrisMetal_SFX.fx
		CThresh 0.1
		Lifespan 500
		LifespanJitter	90
	End
End


Condition
	On	Time
	Time 	40
	Chance	0.5

	Event
		SoftwareOnly
		Ename	parts4
		Type	StartPositOnly
		At	DebrisAnchor
		Geom	Widget_26_Alluminum
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 20 0
			StartJitter		10 10 5
			StartColor		230 230 250
			PyrRotateJitter		180 180 180
			Scale			1 1 1
			Spin			0.0 0.0 0.0
			SpinJitter		0.2 0.2 0.2
			InitialVelocity		0.0 -0.8 0.0
			InitialVelocityJitter	0.3 0.6 0.3
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9
		End
		CEvent 	:MetalDebrisSparks.fx		#V_COV\PhysicsEnabled\ScrapeSparks.fx
							#CEvent	:DebrisMetal_SFX.fx
		CThresh 0.1
		Lifespan 500
		LifespanJitter	90
	End
End

Condition
	On	Time
	Time 	45
	Chance	0.4

	Event
		SoftwareOnly
		Ename	parts5
		Type	StartPositOnly
		At	DebrisAnchor
		Geom	Widget_25_Alluminum
		Bhvr	V_COV\PhysicsEnabled\Icecubes.bhvr #Explosions\TrashExplosion\TrashDebrisFade.bhvr
		BhvrOverride
			PositionOffset		0 20 0
			StartJitter		10 10 5
			StartColor		230 230 250
			PyrRotateJitter		180 180 180
			Scale			0.4 0.4 0.4
			Spin			0.0 0.0 0.0
			SpinJitter		0.2 0.2 0.2
			InitialVelocity		0.0 -0.8 0.0
			InitialVelocityJitter	0.3 0.6 0.3
			Gravity			0.05
			physGravity		1.2
			physRestitution 	0.1
			physSFriction 	 	5
			physDFriction 	 	0.9
		End
		CEvent	:MetalDebrisSparks.fx		#V_COV\PhysicsEnabled\ScrapeSparks.fx
							#CEvent	:DebrisMetal_SFX.fx
		CThresh 0.1
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