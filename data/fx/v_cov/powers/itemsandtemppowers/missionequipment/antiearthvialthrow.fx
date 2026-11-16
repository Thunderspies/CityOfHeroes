#########################################################
##	
##

FxInfo
LifeSpan  180
##################################


Condition
	On	Time
	Time	5

	Event
		Type	Local	
		At	Origin
		Sound vialtoss 80 80 .7
	End
End



Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime1
		Type	local 
		At	wepR
		geom	GEO_WepR_Beer_Bottle_Clear
		bhvr	V_COV\WEAPONFX/DynamiteFade.bhvr
		bhvrOverride
			StartColor	100 10 255
			Alpha		120
		End
		lifespan	28
	End


	Event
		EName 	Fuse
		Type	Local
		At	Prime1		
		Part	:AntiEarthVialGlow.part
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part	
	End


End

Condition
	On 	Time
	Time 	34

	Event
		EName 	Prime
		Type	start 
		At	wepR
		geom	GEO_WepR_Beer_Bottle_Clear
		bhvr	V_COV\WEAPONFX/DynamiteProjectile.bhvr
		bhvrOverride
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
		Part	:AntiEarthVialGlow.part	
		Part	:AntiEarthVialShine.part
		Part	:AntiEarthVialTrail.part
		Part	:AntiEarthVialTrail.part
	End


End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Fire
		Type	Posit
		At	T_Root
		Part2	powers/firecontrol/FireWeaponHitSpark.part
		Lifespan 10
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part1	WEAPONFX/GernadeLauncherSparks.part
		Part2	V_COV\WEAPONFX/DynamiteExplodeSparks.part
		#Sound	grexplo2 120.0 30.0 1.0
		Lifespan 5
		
	End

	Event
		EName 	Flash
		Type	Posit
		At	T_Root
		Part	WEAPONFX/FireExplosionFlash.part		
		Lifespan 5
		
	End

	
	Event
		EName 	Mists
		Type	start
		At	T_Root
		Bhvr	Behaviors/GenericParticleFade.bhvr
		Part	:PurpleMist01.part
		Part	:PurpleMist02.part
		Part	:PurpleMistLight.part
		Part	:PurpleMistDark.part
		Part1	:PurpleMistFlat.part
		LifeSpan	120
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


##################################


End							