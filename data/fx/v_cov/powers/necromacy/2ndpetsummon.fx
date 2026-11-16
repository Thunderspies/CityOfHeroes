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
		
		Part	:MidnightHandTendril_Subtractive.part
		part	:aFearMist.part
		part	:aFearMist2.part
		Part	:aFearTenticles.part
		Part	:aFearTenticles2.part
		part	:aFearMist.part
		part	:aFearMist2.part
		Part	:aFearTenticles.part
		Part	:aFearTenticles2.part
		Part	:MidnightHandTendril.part
		
		Part	:Bones.part
		Part	:Bones1.part
		Part	:Bones2.part

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
		
		
		Part	:ScatterBones.part
		Part	:ScatterBones1.part
		Part	:ScatterBones2.part
		
		lifeSpan	9
	End

	Event	

		ENAME	Base
		Type	start
		At	T_Root
		
		Bhvr 	:DarkScale3.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst 100 100 .6
		
		lifeSpan	80
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	T_Root
		
		Part1	powers/DarknessControl/CreateDarknessTrail.part
		Part2	powers/DarknessControl/CreateDarknessTrail2.part
		Sound miasma_loop 100 100 .5
		
		lifeSpan	20

	End

	Event	

		Type	start
		At	T_Root
		
		Part	:CircleBones.part
		Part	:CircleBones1.part
		Part	:CircleBones2.part
		
		lifeSpan	24

	End

	Event
		EName	darkskirt
		Type	Start
		At	T_Root
		
		
		Part	:2CreateDarknessPuddle2.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat_Subtractive.part
		Part	powers/DarknessControl/CreateDarknessPuddle2Flat.part
		Part	:2CreateDarknessPuddle.part
		
		lifeSpan	20
	End

##############################################################################################

	Event
		EName 	Prime
		Type	Destroy

	End

	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	T_root
		
		BHVR	POWERS\DarknessControl\SplatRotate.bhvr
		
		lifeSpan	20
	End

End

Condition
	On	cycle
	Time	5
	chance	.9

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInDark2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	10
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilA.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	10
		
	End

	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInA.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	10
	End
	
	Event	

		ENAME	BaseTrail1
		Type	start
		At	hookupW/Spin
		
		BHVR	POWERS\DarknessControl\DarktendrilInDark.bhvr

		Splat	DarknessSplat2.tga

		LifeSpan	10
	End
End

End				