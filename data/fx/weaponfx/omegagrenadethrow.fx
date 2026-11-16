#############################################################
## OmegaGrenadeThrow.fx
#############################################################

FxInfo
LifeSpan  200

#############################################################

Input
	InpName	Hips
End

##################################


Condition
	On	Time
	Time	10

	Event
		Type	Local
		At	Origin
		Sound grtoss1 60 60 .7
	End
End



Condition
	On 	Time
	Time 	10

	Event
		EName 	MineStill
		Type	local
		At	wepR
		Anim	v_arachnos_mine
		bhvr	behaviors/grenadeFade.bhvr
		BhvrOverride
			Scale		0.33 0.33 0.33
			PositionOffset	0 -1.5 0
		End
		lifespan	22
	End

End

Condition
	On 	Time
	Time 	32

	Event
		EName 	Prime
		Type	start
		At	wepR
		bhvr	behaviors/MorterLobbprojectile2.bhvr
		Part1	:BeanBagDust2.part
		Magnet	T_Root
		LookAt	T_Root

	End

	Event
		EName	Mine
		Type	Local
		At	Prime
		BhvrOverride
			Scale		0.33 0.33 0.33
			PositionOffset	0 -1.5 0
		End
		Anim	v_arachnos_mine
	End
End


##################################

Condition
	On 	PrimeHit

	Event
		Type	Start
		At	T_Root
		BHVR	Behaviors/CameraShake01Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part1	:FireExplosion.part
		Lifespan 8

	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	:GernadeLauncherSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5

	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:FireExplosionFlash.part
		Lifespan 5

	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	T_Root

		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		20
			PhysForcePower		500
			PhysForcePowerJitter	50
		End
		Lifespan	1

	End



	Event
		Ename 	Prime
		Type	Destroy
	End


	Event
		Ename 	Mine
		Type	Destroy
	End
End

Condition
	On	PrimeHit
	Repeat	24

	Event
		EName	Sparkout
		Type 	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx

	End

	Event
		HardwareOnly
		EName	Sparkout
		Type 	Start
		At	T_Root
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
	End
End

##################################

End