#############################################################
## Fountain_Praetorian.fx
#############################################################

FxInfo

#############################################################

Condition
	On	Time
	Time	0

	Event
		EName	SplashAnchor2
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
	Time 	8

	Event
		SoftwareOnly
		Ename	parts1
		Type	StartPositOnly
		At	SplashAnchor2
		Geom	Rubble_ConcreteSlab_01
		Geom	Rubble_ConcreteSlab_02
		Geom	Rubble_ConcreteSlab_03
		#Bhvr	ConcreteRocks.bhvr #V_COV\PhysicsEnabled\Icecubes.bhvr
		BhvrOverride
			PositionOffset		0 40 0
			Scale			0.02 0.0125 0.02
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

###################################	SPLASH

Condition
	On 	Time
	Time 	28

	Event
		Type	Local
		At	SplashAnchor2
		BhvrOVerride
			PositionOffset	0 0 0
		End
		Part	:Bottom_Splash_Small.part
		Lifespan 2
	End

	Event
		Type	Local
		At	SplashAnchor2
		BhvrOVerride
			PositionOffset	0 -1 0
		End
		Part	:Splash_Small_Base.part
		Lifespan 2
	End

	Event
		Type 	Local
		At	SplashAnchor2
		BhvrOVerride
			PositionOffset	0 0.5 0
		End
		Part	:Bottom_Splash_Flat_Small.part
		Part	:Bottom_Foam_Flat_Small.part
		Lifespan 45
	End
End

Condition
	On 	Time
	Time 	36

	Event
		Type	Local
		At	SplashAnchor2
		BhvrOVerride
			PositionOffset	0 0 0
		End
		Part	:Bottom_Splash_Secondary_Sm.part
		Part	:Splash_Small_Thick01.part
		#Part	:Splash_Small_Thick02.part
		Lifespan 2
	End
End



#############################################################

End