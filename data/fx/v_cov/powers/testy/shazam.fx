#########################################################
##	Shazam
##
FxInfo

LifeSpan	200

#Condition
#	Event
#		EName	InitialSwordFire
#		Type	Local 
#		At	WepR
#		Part	:FireHands.part
#		#Bhvr	:testpivot.bhvr
#		Sound	Fireball1 100.0 30.0 .8
#		LifeSpan 45
#	End
#End
#
#Condition
#	On 	Time
#	Time 	10
#	Event
#		EName	SwordFlash2
#		Type	Local
#		At	WepR
#		Part1	:FireHandsFlash.part
#		LifeSpan 14
#	End
#End

Condition
	On 	Time
	Time 	16

	Event
		EName 	Prime
		Type	Start 
		At	WepR
		Bhvr	:projectileWild01.bhvr
		Magnet	Target
		#LookAt	Target
		ChildFx	:ShazamOrbitChild.fx
		Part	:FireBallHead.part
		Part	:ShazamSpinner.part
		Part	:FireBallStreak.part
		Part	:ShazamSparkTrail.part
	End

End

Condition
	On	PrimeHit
			
	Event
		Ename	FireBallHit
		Type	local
		At	target

		Part	:BigFireExplosion.part
		Part	:ShazamImpact.part
		Part	:ShazamFlash.part
		Part	:FireBallHitSpark.part
	End

	Event
		Ename	Prime
		Type	Destroy
	End

End

End	


			