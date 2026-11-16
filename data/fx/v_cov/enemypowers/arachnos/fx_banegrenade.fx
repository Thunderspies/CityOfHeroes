#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	160

#########################################################

Condition
	On	Time
	Time	0

	Event
		EName	HoleStart
		Type	Local
		At	Root
	
		bhvrOverride
			PositionOffset	0 .25 5.3
		End

#		Geom	Testing_Target
	End

	Event
		EName	Hole
		Type	Start
		At	HoleStart

		bhvrOverride
			StartColor	10 10 10
			Alpha		150
		End

		Geom	GEO_CircleGuide
	End
End


Condition
	On 	Time
	Time 	24

	Event
		EName	BaneGrenade
		Type	Local
		At	WepR
		
		Bhvr	V_COV\EnemyPowers\Arachnos\B_Wep_BaneGrenade.bhvr
		Geom	GEO_WEPR_GRENADE_1
		Lifespan 80
	End

End

Condition
	On	Time
	Time	100

	Event
		EName	ExplosionSpot
		Type	Start
		At	WepR
		bhvrOverride
			PositionOffset	0 1 2
		End

#		Geom	Testing_Target
	End
End

####################################################################################################
#########################################  Explosion  ##############################################
####################################################################################################

Condition
	On 	Time
	Time	125

	Event
		EName 	Fire
		Type	Posit
		At	Hole

		bhvr	behaviors\CameraShake02Subtle.bhvr
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke1.part
		Part1	V_COV\WeaponFx\DynamiteExplodeSmoke2.part
		Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 10
	End

	Event
		EName 	Sparks
		Type	Posit
		At	Hole
		Part1	WEAPONFX/GernadeLauncherSparks.part
		Part2	V_COV\WeaponFx\DynamiteExplodeSparks.part
		Sound grexplo2 120.0 120.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Flash
		Type	Posit
		At	Hole
		Part	WEAPONFX/FireExplosionFlash.part		
		Lifespan 5
		
	End
End




####################################################################################################
########################################  Ground Burst  ############################################
####################################################################################################

Condition
	On 	Time
	Time 	125

	Event
		EName	Ring10
		Type	Local
		At	Hole
		
		Bhvr	V_COV\Powers\RobotSpawns\B_Dust_BaneBombHole.bhvr
		Part	V_COV\Powers\RobotSpawns\P_Dust_BaneBombHole.part
		LifeSpan	1		
	End

End

#Condition
#	On 	Time
#	Time 	130
#
#	Event
#		EName	Ring10
#		Type	Local
#		At	Hole
#		
#		Bhvr	V_COV\Powers\RobotSpawns\B_Dust_BaneBombHole.bhvr
#		Part	V_COV\Powers\RobotSpawns\P_Dust_BaneBombHole.part
#		LifeSpan	1		
#	End
#
#End

End				