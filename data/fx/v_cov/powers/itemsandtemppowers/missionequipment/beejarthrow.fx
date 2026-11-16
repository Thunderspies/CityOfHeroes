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
		Sound BeeJarToss 80 80 .7
	End
End



Condition
	On 	Time
	Time 	5

	Event
		EName 	Prime1
		Type	local 
		At	wepR
		lifespan	28
	End


	Event
		EName 	Jar1
		Type	local 
		At	Prime1
		geom	baseliquid_mbottlesm01b_f_n_p #Med_beaker_03 #GEO_WepR_Beer_Bottle_Clear
		bhvr	V_COV\WEAPONFX/DynamiteFade.bhvr
		bhvrOverride
			PositionOffset	-.1 -.1 -.5
			pyrRotate	-90 0 0
			Alpha		250
		End
	End

	Event
		EName 	Fuse
		Type	Local
		At	Prime1		
		Part	:BeesSmall.part  
		Part	:BeeJarGlow.part
		#Part	:AntiEarthVialShine.part
		Part	:BeeTrail02.part	
	End


End

Condition
	On 	Time
	Time 	34

	Event
		EName 	Prime
		Type	start 
		At	wepR
		bhvr	V_COV\WEAPONFX/DynamiteProjectile.bhvr
		bhvrOverride
			TrackRate	2
		End
		Magnet	T_Root
		LookAt	T_Root
		

	End

	Event
		EName 	Jarr
		Type	Local 
		At	Prime
		geom	baseliquid_mbottlesm01b_f_n_p #Med_beaker_03
		bhvrOverride
			PositionOffset	0 0 -.5
			pyrRotate	-90 0 0
			Alpha		250
		End

	End

	Event
		EName 	Fuse
		Type	Local
		At	Prime
		Part	:BeesSmall.part
		Part	:BeesSmall.part
		Part	:BeeJarGlow.part	
		#Part	:AntiEarthVialShine.part
		#Part	:AntiEarthVialTrail.part
		Part	:BeeTrail.part
	End


End


##################################

Condition
	On 	PrimeHit

	Event
		EName 	Explode
		Type	Posit
		At	T_Root
		Part	:BeeTrail.part
		Part	:BeeMist.part
		Lifespan 10
	End

	Event
		EName 	Sparks
		Type	Posit
		At	T_Root
		Part	:BeeTrail.part
		Part	:BeeMist2.part
		Sound BeeJarExplode 80.0 80.0 .9
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
		Part	:BeeSwarm.part		
		Part	:BeeTrail.part
		Part	:BeeMist.part
		Part	:BeesCoreMist.part
		Part	:BeesCoreMist2.part
		Pmagnet	T_hips
		LifeSpan	60
	End

	Event
		Ename 	Prime
		Type	Destroy
	End

End


##################################


End							