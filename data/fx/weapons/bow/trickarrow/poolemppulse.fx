#########################################################
##	Fireweapon
########################################################
FxInfo


LifeSpan	70
######################################################################

Condition
	On	Time
	Time	0

	Event
		Type	Local
		At	Chest
		Sound EMPLuminary 90 90 .9
	End
End



Condition
	On 	Time
	Time 	18

	Event
		EName 	Mallet1
		Type	local 
		At	Chest
		bhvr	:EMPBubbleScale.bhvr
		Geom	ElectricalMeleeBubble
		
		LifeSpan	5
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		sound	arrowhitEMP 80 70 .8
		Part	:Dots.part
		Lifespan	10
	End

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		Part	:HitEmber.part
		Part	:HitStar.part
		#Part	:HitEmberFlat.part
		#Part	:HitStarFlat.part
		Part	:HitRing.part
		Part	:HitRing2.part
		Part	:HitGlows2.part
	
		#Sound	PunchHit2 100.0 30.0 .8
	End

End

Condition
	On 	Time
	Time 	20

	Event
		EName 	Mallet2
		Type	start 
		At	chest
	
		Part	:EMPElectricityArch1.part
		Part	:EMPElectricityArch2.part
		LifeSpan	8
	End

End


Condition
	On 	Time
	Time 	25

	Event
		EName	Mallet1
		Type	Destroy
		
	End
	
	
End


End
##################################

