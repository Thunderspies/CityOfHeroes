#########################################################
##	
##

FxInfo

LifeSpan	200

##################################

COndition
	On	Time
	Time	13

	Event
		Type	Local
		At	Origin
		
		Sound VialToss 70 70 .8
	End

	Event
		Type	Local
		At	WepR
		
		bhvr	:HandBeaker.bhvr
		Geom	Med_Beaker_03
		LifeSpan	34

	End
End

Condition
	On 	Time
	Time 	34	#16

	Event
		EName	Prime
		Type	Start
		At	WepR

		Part	:PowerUpSteamAdditive.part
		Part	:PowerUpStreamDust02.part
		Part	:PowerUpMainStreamChunks01.part
		bhvr	:PowerUp.bhvr

		Magnet	T_Root
		LookAt	T_Root
	End
	
	Event
		EName	Prime2
		Type	Start
		At	WepR

		Geom	Med_Beaker_03
		bhvr	:PowerUp2.bhvr

		Magnet	T_Root
		LookAt	T_Root
	End
End

Condition
	On 	PrimeHit

	Event
		Ename 	Prime
		Type	Destroy
	End

	Event
		Ename 	Prime2
		Type	Destroy
	End

	Event
		Type	Start
		At	T_Root
		ChildFx	Explosions\GlassExplosion\GlassExplosion_Smallest.fx
		LifeSpan	230
	End
End


Condition
	On 	Time
	Time 	34	#0

	Event
		EName	Dust
		Type	Local
		At	wepR
		Part	:PowerUpSubtractiveHand.part
		Part	:PowerUpStreamDust01.part

		LifeSpan 10
	End
End



Condition
	On 	PrimeHit

	Event
		EName	DustHit
		Type	Start
		At	T_Root
		Part	:PowerUpHitDust02.part
		Part	:PowerUpHitSparkles01.part
		Part	:PowerUpChunkBlast01.part
		Part	:PowerUpHitDust01.part
		LifeSpan 30
	End

#	Event
#		EName	DustHit
#		Type	Start
#		At	T_Root
#		Part	:PowerUpHitDustRing01.part 
#		LifeSpan 40
#	End

	Event	
		ENAME	Ring
		Type	Start
		At	T_Root
		
		Splat	MistRing01.tga
		Bhvr	:Splat.bhvr

		Lifespan 8
	End

End


End

	