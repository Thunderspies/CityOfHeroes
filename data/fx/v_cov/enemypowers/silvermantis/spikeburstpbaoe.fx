#########################################################
##	
##

FxInfo

LifeSpan	87

##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	HandL
		Sound ThornRumble 70 70 .8
	End
End

Condition
	On 	Time
	Time 	44



	Event
		
		Type	Local
		At	HandL
		Sound thornblast5 80 80 .95
	End
End

##################################

Condition
	On	Time
	Time	1

	Event
		Ename	Spike00
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	48
	End


	Event
		Ename	Spike01
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	48
	End


	Event
		Ename	Spike02
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	48
	End

End
Condition
	On	Time
	Time	4

	Event
		Ename	Spike03
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	45
	End


	Event
		Ename	Spike04
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	45
	End


	Event
		Ename	Spike05
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	45
	End

End
Condition
	On	Time
	Time	7

	Event
		Ename	Spike06
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	42
	End


	Event
		Ename	Spike07
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	42
	End


	Event
		Ename	Spike08
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	42
	End

End

Condition
	On	Time
	Time	10

	Event
		Ename	Spike09
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	39
	End

	Event
		Ename	Spike10
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	39
	End

	Event
		Ename	Spike11
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	39
	End

End

Condition
	On	Time
	Time	13

	Event
		Ename	Spike12
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	36
	End

	Event
		Ename	Spike13
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	36
	End

	Event
		Ename	Spike14
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	36
	End

End

Condition
	On	Time
	Time	16

	Event
		Ename	Spike15
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	33
	End

	Event
		Ename	Spike16
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	33
	End


	Event
		Ename	Spike17
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	33
	End


End

Condition
	On	Time
	Time	19

	Event
		Ename	Spike18
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	30
	End

	Event
		Ename	Spike19
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	30
	End

	Event
		Ename	Spike20
		Type	Local
		At	Chest
		Bhvr	:SpikeBurstFade.bhvr
		Geom	FX_SilverSpike02
		Lifespan	30
	End

End



#######################################################################
##Spike / Burst#######################################################
##################################

Condition
	On	Time
	Time	48

	Event
		EName 	Pulse
		Type	SetLight
		Bhvr	:silverLights.bhvr
		LifeSpan 15
	End

	Event
		EName 	Explode 
		Type	Start
		At	Chest
		Part	:silverFlash1.part		
		Part	:silverSparks1.part
		LifeSpan 5
	End

	Event
		Ename	SpikeShot00
		Type	Local
		At	Spike00
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot01
		Type	Local
		At	Spike01
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot02
		Type	Local
		At	Spike02
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot03
		Type	Local
		At	Spike03
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot04
		Type	Local
		At	Spike04
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot05
		Type	Local
		At	Spike05
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot06
		Type	Local
		At	Spike06
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot07
		Type	Local
		At	Spike07
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot08
		Type	Local
		At	Spike08
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot09
		Type	Local
		At	Spike09
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot10
		Type	Local
		At	Spike10
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot11
		Type	Local
		At	Spike11
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot12
		Type	Local
		At	Spike12
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot13
		Type	Local
		At	Spike13
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot14
		Type	Local
		At	Spike14
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End
	
	Event
		Ename	SpikeShot15
		Type	Local
		At	Spike15
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot16
		Type	Local
		At	Spike16
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot17
		Type	Local
		At	Spike17
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot18
		Type	Local
		At	Spike18
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot19
		Type	Local
		At	Spike19
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End

	Event
		Ename	SpikeShot20
		Type	Local
		At	Spike20
		Bhvr	:SpikeBurstProjectile.bhvr
		Geom	parent
		Part	:SpikeTrailSmoke.part
		Part	:SpikeTrailSmoke2.part		
		Part	:silverSparkTrail.part
	End


End

End				