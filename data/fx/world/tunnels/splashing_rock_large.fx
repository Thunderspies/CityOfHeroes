#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	SplashAnchor1
		Type	Local
		At	Root
		BhvrOverride
			PositionOffset	0 0 0
		End
	End
End

#####################################

Condition
	On	Time
	Time 	0

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	SplashAnchor1
		Geom	Rubble_ConcreteSlab_01
		Geom	Rubble_ConcreteSlab_02
		Geom	Rubble_ConcreteSlab_03
		#Bhvr	ConcreteRocks.bhvr #V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0 40 0
			Scale			0.03 0.025 0.03
			InitialVelocity		0.0 -1.5 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.1 0.15 0.1
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
		#ChildFX :DebrisMetal_SFX.fx
		#CEvent	:CHILD_RockBreak.fx
		#CEvent	:DebrisConcrete_SFX.fx
		Lifespan 20
	End
End

############

Condition
	On	Time
	Time 	3
	Chance	0.5

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	SplashAnchor1
		Geom	Rubble_ConcreteSlab_01
		Geom	Rubble_ConcreteSlab_02
		Geom	Rubble_ConcreteSlab_03
		#Bhvr	ConcreteRocks.bhvr #V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0.5 40 1
			Scale			0.02 0.0125 0.02
			InitialVelocity		0.0 -1.5 0.0
			InitialVelocityJitter	0.0 0.0 0.0
			Spin			0.0 0.1 0.0
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
		#ChildFX :DebrisMetal_SFX.fx
		#CEvent	:CHILD_RockBreak.fx
		#CEvent	:DebrisConcrete_SFX.fx
		Lifespan 20
	End
End

###################################	SPLASH

Condition
	On 	Time
	Time 	20

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0 0.0
		End
		Part	:Bottom_Splash_Large.part
		Lifespan 2
	End

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0 0.0
		End
		Part	:Splash_Base.part
		Part	:Splash_Base02.part
		Lifespan 2
	End

	Event
		Type 	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0.5 0.0
		End
		Part	:Bottom_Splash_Flat_Large.part
		Part	:Bottom_Foam_Flat.part
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	28

	Event
		Type	Local
		At	SplashAnchor1
		BhvrOVerride
			PositionOffset	0.0 0 0.0
		End
		Part	:Bottom_Splash_Secondary.part
		Part	:Splash_Thick01.part
		Part	:Splash_Thick02.part
		Part	:Splash_Thick03.part
		Lifespan 2
	End
End


#############################################################

End