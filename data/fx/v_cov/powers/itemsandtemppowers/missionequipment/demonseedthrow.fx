#########################################################
##	
##

FxInfo
LifeSpan  180
##################################


Condition
	On	Time
	Time	9

	Event
		Type	Local	
		At	Origin
		Sound caltropstoss 80 80 .6
	End
End





Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime1
		Type	local 
		At	wepR
		#geom	GEO_WepR_Beer_Bottle_Clear
		bhvr	V_COV\WEAPONFX/DynamiteFade.bhvr
		lifespan	14
	End


	Event
		EName 	Fuse
		Type	Local
		At	Prime1		
		Part	:DemonSeedGlow.part
		Part	:DemonSeedShine.part
		Part	:DemonSeedTrail.part	
	End


End

Condition
	On 	Time
	Time 	19

	Event
		EName 	Prime
		Type	start 
		At	wepR
		#geom	GEO_WepR_Beer_Bottle_Clear
		bhvr	V_COV\WEAPONFX/DynamiteProjectile.bhvr
		bhvrOverride
			TrackRate	1.8
			StartColor	100 10 255
			Alpha		120
		End
		Magnet	T_Root
		LookAt	T_Root
		

	End

	Event
		EName 	Fuse
		Type	Local
		At	Prime		
		Part	:DemonSeedGlow.part	
		Part	:DemonSeedShine.part
		Part	:DemonSeedTrail.part
		Part	:DemonSeedTrail.part
	End


End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part2	V_COV\WEAPONFX/DynamiteExplodeSparks.part
		Sound ExplosiveAppearance 80 80 .86
		Lifespan 5
		
	End

	Event
		EName 	Flash
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part	
		Part	:DemonSeedRays.part	
		Part	:DemonSeedExplode.part		
		Lifespan 5
		
	End

	
	Event
		EName 	Mists
		Type	start
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:RedMist01.part
		Part	:RedMist02.part
		Part	:RedMistLight.part
		Part	:RedMistDark.part
		Part1	:RedMistFlat.part
		LifeSpan	45
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


##################################


End							