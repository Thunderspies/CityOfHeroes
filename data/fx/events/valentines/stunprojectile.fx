#########################################################
##	
##

FxInfo

LifeSpan	200

##################################

COndition
	On	Time
	Time	0

	Event
		Type	Local
		At	Origin
#		Sound	Effigy5 100 90 .75
	End
End


Condition
	On 	Time
	Time 	16

	Event
		EName	Prime
		Type	Start
		At	WepR

		Part	:PowerUpMainStreamRays.part
		Part	:StunStream.part
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
	Time 	0

	Event
		EName	Dust
		Type	Local
		At	wepR
		Part	:PowerUpSubtractiveHand.part
		Part	:PowerUpStreamDust01.part

		LifeSpan 20
	End
End



Condition
	On 	PrimeHit

	Event
		EName	DustHit
		Type	Start
		At	T_Root
		Part	:PowerUpChunkBlast01.part
#		Part	:PowerUpHitDust01.part
		LifeSpan 10
	End

#	Event
#		EName	DustHit
#		Type	Start
#		At	T_Root
#		Part	:PowerUpHitDustRing01.part 
#		LifeSpan 40
#	End
#
#	Event	
#		ENAME	Ring
#		Type	Start
#		At	T_Root
#		part	:PowerUpFlatRing01.part
#		Lifespan 10
#	End

End


End

	