#########################################################
##	chill touch hit
FxInfo

Input  
	InpName	wepR
End

Input  
	InpName	wepL
End

Input  
	InpName	T_Root
End

LifeSpan	200

##################################################################################

Condition
	On	Time
	Time	0
		
	Event	

		ENAME	Base
		Type	local
		At	WepR
		Sound darkwindup2 60 60 .3
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	40

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	40
	End

End

Condition
	On	Time
	Time	17
	

	Event
		EName	Prime
		Type	start
		At	wepL
		Bhvr	Behaviors/MorterLobbProjectile.bhvr
		
		part	:aFearMist.part
		part	:aFearMist2.part
		Part	:aFearTenticles.part
		Part	:aFearTenticles2.part
		part	:aFearMist.part
		part	:aFearMist2.part
		Part	:aFearTenticles.part
		Part	:aFearTenticles2.part
		Part	:MidnightHandTendril.part

		Sound MidnightGrasp 70 70 .85
		Magnet	T_Root
		Lookat	T_Root
	End

End


Condition
	On 	PrimeHit
	
	
	Event
		Type	Local
		At	T_Root
		
		ChildFx  :SummonHit.fx
		
		lifeSpan	200
	End

#	Event	
#
#		ENAME	Base
#		Type	start
#		At	T_Root
#		
#		Bhvr 	:DarkScale3.bhvr
#		Geom	FX_DarkWave
#		Sound	miasmaburst 100 30 .6
#		
#		lifeSpan	40
#	End
#
#	Event	
#
#		ENAME	BaseTrail
#		Type	start
#		At	T_Root
#		
#		Part1	powers/DarknessControl/CreateDarknessTrail.part
#		Part2	powers/DarknessControl/CreateDarknessTrail2.part
#		Sound	miasma_loop 100 30 .5
#		
#		lifeSpan	20
#
#	End
#
#	Event	
#
#		Type	start
#		At	T_Root
#		
#		Part	:CircleBones.part
#		Part	:CircleBones1.part
#		Part	:CircleBones2.part
#		
#		lifeSpan	20
#
#	End
#
#	Event
#		EName	darkskirt
#		Type	Start
#		At	T_Root
#		
#		Part	powers/DarknessControl/CreateDarknessPuddleFlat.part
#		Part	powers/DarknessControl/CreateDarknessPuddle2Flat.part
#		Part	:CreateDarknessPuddle.part
#		Part	:CreateDarknessPuddle2.part
#		
#		lifeSpan	20
#	End

##############################################################################################

	Event
		EName 	Prime
		Type	Destroy

	End

End

End				