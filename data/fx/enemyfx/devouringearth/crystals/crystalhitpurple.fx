#########################################################
##	
##

FxInfo

Input  
	InpName	neck
End

Input  
	InpName	head
End

Input  
	InpName	chest
End

Input  
	InpName	hips
End

Input  
	InpName	UArmL
End

Input  
	InpName	UArmR
End

LifeSpan	50
##################################
##################################
##################################

Condition
	On 	Time
	Time 	0



	Event
		
		Type	Local
		At	chest
		Sound Crystalshit 70 70 .7
	End
End

##################################
##################################
##################################

Condition
	On 	Time
	Time 	0

	Event	
		ENAME	CrossHairNode
		Type	Local
		At	Chest
		Part1	Powers/MartialArts/MAHitStar01.part
		Part2	Powers/MartialArts/MAHitStar02.part
		Part3	Powers/MartialArts/MAHitStar03.part
		Part4	Powers/MartialArts/MAHitGlow01.part
		Part5	Powers/MartialArts/MAHitRays01.part
		Lifespan 200
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill01p
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill03p
		
	End

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill07p
		
	End

	

	Event

		
		Type	Local
		At	chest
		Bhvr	Behaviors/QuillProjectile4.bhvr
		Geom	Fx_quill06p
		#Sound	poisonhit2 50 30 .7
	End

	

	Event

		EName	Gas
		Type	Local
		At	chest
		#Part	:QuillsScatter.part		
		#Part1	:MustardGasHitA.part
		
		LifeSpan	30
		
	End

		
End



End				