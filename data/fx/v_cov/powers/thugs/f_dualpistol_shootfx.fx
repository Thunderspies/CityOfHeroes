#########################################################
##	Dual Pistols AoE
##

FxInfo

LifeSpan 400

#################################################################################
####   Clip Toss and Target   ###################################################
#################################################################################

Condition
	On 	Time
	Time 	0

	Event
		Type	Local
		At	origin
		Sound uziclipout 70 70 .56
	End
End




Condition
	On	Time
	Time	0

	Event
		EName	ClipTarget
		Type	Start
		At	Target
		bhvrOverride
			positionOffset	0 3 0
		End
	
#		Geom	Testing_TargetB
	End
End


Condition
	On	Time
	Time	50
	
	Event
		EName	Prime
		Type	Start
		At	WepR
		bhvrOverride
			PyrRotate  90 0 0
		End
		
		Bhvr	:B_Travel_ClipToss.bhvr			
#		Geom	Testing_Target
		Geom	Geo_MicroUzi_Clip
		Magnet	 ClipTarget
	End
End

#################################################################################
####   Clip Toss and exp   ######################################################
#################################################################################

Condition
	On 	PrimeHit
	
	Event
		Type	Start
		At	Prime
		BHVR	Behaviors/CameraShake01Subtle.bhvr
	End

	Event
		EName 	Fire
		Type	Posit
		At	Prime
		Part1	WeaponFX\FireExplosion.part
		#Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 8
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Prime
		Part1	WeaponFX\GernadeLauncherSparks.part
		#Part2	WeaponFX\GernadeLauncherSpark.part
##		Sound	grexplo2 120.0 30.0 1.0  # needs sound to better fit timing
		Lifespan 5
		
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Prime
		Part1	WeaponFX\FireExplosionFlash.part
		Part2	WeaponFX\GernadeLauncherSparks2.part
		Part3	WeaponFX\SmokeExplosion.part
		Part4	WeaponFX\FireExplosion2.part
		
		Lifespan 10	
	End
	
	Event
		EName 	RadiusFxScale
		Type	start 
		At	Prime

		bhvr    behaviors/powers/firecontrol/InfernobubbleA.bhvr
		Part	powers/firecontrol/FireBallHitRing1a.part
		Part	powers/firecontrol/FireBallHitRing3a.part
		Part	powers/firecontrol/InfernoExplosion.part
		LifeSpan	8
		
	End

End

Condition
	On	Primehit
		
	Repeat	24
	
	Event
		EName	Sparkout
		Type 	Start
		At	Prime
		BhvrOverride
			PositionOffset	0 1.5 0
		End
		ChildFX	V_COV\PhysicsEnabled\ShrapnelSparkExplode.fx
		
	End

End


##################################

Condition
	On 	PrimeHit
	
	Event
		Ename 	Prime
		Type	Destroy
	End


End


#################################################################################
####   GUN FIRE from uzi   ######################################################
#################################################################################

Condition
	On	Time
	Time	56

	Event
		EName	Gunfire
		Type	Local
		At	HandR
		Sound uziburst4b 80 80 .88
		ChildFX	:F_ChildGunfire_ThugUzi.fx
		Lifespan 60
#		Geom	Testing_Target
	End
End


Condition	
	On	Time
	Time	56

	Event
		EName	Shells
		Type	Local
		At	HandR
		bhvrOverride
			PositionOffset		.2 .1 .35
			PYRRotate		90 0 90
		End

		Childfx	:F_Shells_UziEject.fx
#		Geom	Testing_TargetB
	End
End

End