#########################################################
##	chill touch hit
FxInfo

#Lifespan	60

Input  
	InpName	root
End

Input  
	InpName	WepR
End

Input  
	InpName	WepL
End
#########################################################
##	chill touch hit
FxInfo

#Lifespan	60

##################################
Condition
	On	cycle
	Time	15

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:Darktendril2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
		
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilInDark.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
	End

	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarktendrilIn2.bhvr

		Splat	DarknessSplat.tga

		LifeSpan	30
	End

	
	Event	

		ENAME	BaseTrail
		Type	start
		At	hookupW/Spin
		
		BHVR	:DarkSplat2.bhvr

		Splat	Ember02.tga
		LifeSpan	30
	End

End

Condition
	On	cycle
	Time	19

	Event	

		ENAME	BaseTrail
		Type	start
		At	root

		Bhvr	:splat.bhvr
		splat	Generic_Ring.tga
		LifeSpan	30
	End


End

Condition
	On	Time
	Time	0

	Event
		EName 	burst
		Type	start 
		At	Root
		bhvr    behaviors/powers/firecontrol/Infernobubble.bhvr
		Part1	:PitBust1.part
		Part2	:PitBust2.part
		Sound grexplo2 80 80 1.0
		
	End		

	Event	

		ENAME	Base
		Type	start
		At	root
		
		Bhvr 	Behaviors/DarkScale2.bhvr
		Geom	FX_DarkWave
		Sound miasmaburst4 80 80 .62

	End
	
#	Event	
#
#		ENAME	BaseTrail
#		Type	start
#		At	root
#
#		Bhvr	:splat.bhvr
#		splat	Generic_Ring.tga
#		
#	End
	
	Event	

		ENAME	hookupW/Spin
		Type	posit
		At	root
		
		BHVR	:SplatRotate.bhvr
		Sound Tar_loop 80 80 .53
	End

End

Condition
	On	Time
	Time	30

	Event	

		ENAME	BaseTrail
		Type	start
		At	root

		Part3	:TarBubblePopping.part
		Part5	:TarBubblePoppingFlat.part
		
	End
	
End

End			

##################################
