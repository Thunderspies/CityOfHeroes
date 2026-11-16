#########################################################
##	
##

FxInfo

##################################

Condition
	On 	cycle
	Time 	40
	Chance	1

#	Event	
#		Type	start
#		At	Root
#		BHVR	:BlackRingSplat.bhvr
#		
#		Splat	ThinRing.tga
#		LifeSpan	10
#	End

	Event	
		Type	start
		At	Root
		BHVR	:GreenRingSplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	15
	End

	Event	
		Type	start
		At	Root
		BHVR	:GreenRingsplatBright.bhvr
		
		Splat	Generic_Ring.tga
		LifeSpan	15
	End
	
	Event	
		Type	start
		At	Root
		BHVR	:GreenRingSplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	15
	End

End

Condition
	On 	Time
	Time 	0

#	Event	
#		Type	start
#		At	Root
#		BHVR	:BlackRingSplat.bhvr
#		
#		Splat	ThinRing.tga
#		LifeSpan	10
#	End

	Event	
		Type	start
		At	Root
		BHVR	:GreenRingSplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	15
	End

	Event	
		Type	start
		At	Root
		BHVR	:GreenRingSplat.bhvr
		
		Splat	Coffee_Ring.tga	#Generic_Ring.tga
		LifeSpan	15
	End

	Event	
		Type	start
		At	Root
		BHVR	:GreenRingsplatBright.bhvr
		
		Splat	Generic_Ring.tga
		LifeSpan	15
	End

	Event
		Ename	dust
		Type	posit 
		At	root
		#Bhvr	:VineFade.bhvr
		Part	:TreeDust.part
		Part	:TreeDust2.part

		LifeSpan	100

	End

	Event	
		Type	start
		At	Root
		BHVR	:TreeOfLifeLeavesScale.bhvr
		Geom	TreeOfLife_leaves
		
	End
	
	Event	
		Type	start
		At	Root
		BHVR	:TreeOfLifeLeavesScale.bhvr
		Geom	TreeOfLife_leaves2
		
	End
	
	Event	
		Type	start
		At	Root
		BHVR	:TreeOfLifeLeavesScale.bhvr
		Geom	TreeOfLife_leaves3
		
	End

	Event	
		Type	start
		At	Root
		BHVR	:TreeOfLifeScale.bhvr
		Geom	TreeOfLife
	End


End
	

End				