#########################################################
##	jack
##

FxInfo

Lifespan	100

Flags    InheritAlpha


Condition
	On 	Time
	Time 	10

	Event
		EName 	SpearGrab
		Type	Local 
		At	WepL
		
		Geom	FX_Valkyrie_Spear

		
		Bhvr	:SpearWeaponFadeIn.bhvr
		BhvrOverride
			pyrRotate	180 0 0
		End
		Sound arrowout8 90 90 .76

		Lifespan	35
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Glow
		Type	Local 
		At	SpearGrab
		POther	SpearGrab
		
		BhvrOverride
			PositionOffset	0 0 2
		End

		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:SpearOffset1.bhvr
	End
End

###############  Tracking Projectile   #############

Condition
	On 	Time
	Time 	45

	Event
		EName 	Prime
		Type	Start
		At	WepL
		Bhvr	:SpearTrackingProjectile.bhvr
		Magnet	Target
		Lookat	Target
		Lifespan	60
	End

	Event
		EName 	Spear
		Type	Local 
		At	Prime
		
		Geom	FX_Valkyrie_Spear
		BhvrOverride
			PositionOffset	0 0 -1
		End

		Bhvr	:SpearOffset1.bhvr
	End
	
	Event
		EName 	Glow
		Type	Local 
		At	Spear
		POther	Spear
		
		Part	:SwordGlow1.part
		Part	:SwordGlow2.part
		Bhvr	:BladeOffset1.bhvr
	End
End

######################################################


Condition
	On 	PrimeHit
	
	Event
		EName 	Prime
		Type	Destroy
	End

	Event
		EName	Explosion
		Type 	StartPositonly
		At	Target
		Part	WEAPONFX/FireExplosionFlash.part
		part	Explosions\medium\FireExplosion2.part
		Lifespan 4
	End

	Event
		EName	Smoke
		Type 	StartPositonly
		At	Target
		part	:SpearHitRing01.part
		part	:SpearHitRing02.part
		part	powers/firecontrol/FireWeaponHitSpark.part
		part	Explosions\medium\SmokeExplosion.part
		Lifespan 10
	End

	Event
		Ename	ExplosiveForce
		Type	Start
		At	Target
		
		BhvrOverride
			PhysForceType		Out
			PhysForceRadius		25
			PhysForcePower		600
			PhysForcePowerJitter	100
		End

		Lifespan	1

	End
End

Condition
	On 	PrimeHit
	
	Repeat	7
	
	Event
		EName	Sparkup
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplodeUP.fx
		
	End

End

Condition
	On 	PrimeHit
	
	Repeat	21
	
	Event
		EName	Sparkup
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\MetalSparkExplode.fx
		
	End

End

Condition
	On 	PrimeHit
	
	Repeat	3
	
	Event
		EName	Sparkup
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\MetalSparkUP.fx
		
	End

End

Condition
	On 	PrimeHit

	Repeat	15
	
	Event
		EName	Sparkfar
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUTbig.fx
		
	End

End

Condition
	On 	PrimeHit
	
	Repeat	3
	
	Event
		EName	Sparkout
		Type 	Start
		At	Target
		ChildFX	V_COV\PhysicsEnabled\MetalSparkOUT.fx
		
	End


End

	
End	