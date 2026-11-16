#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	200
#########################################################

Condition
	On	Time
	Time	10

	Event
		EName	Prime
		Type	start
		At	eyes

		Bhvr	behaviors/Quillprojectile1.bhvr
		Magnet	Target
		LookAt	Target
		
		
		Part	:EnergyHead.part
		Part	:EnergyHeadInvert.part
		Part	:RapidEnergyCoreLocal.part
		Part	:RapidCoreLocal.part
		Part	:RapidCoreLocalInvert.part

		Part	:BotTrail.part
		Part	:BotTrailSpecks.part
		Part	:BotBubblesTrail.part
		Part	:BotBallzTrail.part
		Part	:EnergyStreakTrailStraight.part
		Part1	:BotTrail2.part
		Sound PoliceBotLaser2 100 100 .95
		
	End

	
End

Condition
	On 	PrimeHit
	
	Event
		Type	start
		At	T_Chest

		Childfx		POWERS/MartialArts/MartialArtsHitsBig2.fx
		LifeSpan	3
	End
	
	Event
		EName 	Rings
		Type	start
		At	T_Root

		BHVR	Behaviors/RootToChestadjustment.bhvr
		Part	:HitRingflashthin.part
		Part	:HitRingflashthin2.part
		Part	:HitRingflash.part
		LifeSpan	3
	End
	
	Event
		EName   R
		Type	local
		At	Rings
		Geom	Centerdummy
		Bhvr	:Inferno.bhvr
	End


	Event
		Type	Local 
		At	R
		altpiv	1
		bhvr    :Infernobubble.bhvr
		Part	:aoeDarkEnergy.part
		Part	:aoeLightEnergy.part
		Part1	:aoeEnergy.part
		Part	:aoeEnergyDots.part
	End		

	Event
		EName 	Prime
		Type	Destroy
		

	End

End

End				