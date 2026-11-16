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
		bhvr	WEAPONS\Bow\TrickArrow\EMPBubbleScale.bhvr
		bhvrOverride
			StartColor	0 150 255
		End
		Geom	ElectricalMeleeBubble
		
		LifeSpan	5
		
	End

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		sound	arrowhitEMP 80 70 .8
		#Part	WEAPONS\Bow\TrickArrow\Dots.part
		Lifespan	10
	End

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		Part	:HitEmber.part
		Part	:HitStar.part
		#Part	WEAPONS\Bow\TrickArrow\HitEmberFlat.part
		#Part	WEAPONS\Bow\TrickArrow\HitStarFlat.part
		Part	:HitRing.part
		Part	:HitRing2.part
		Part	WEAPONS\Bow\TrickArrow\HitGlows2.part
	
		#Sound	PunchHit2 100.0 30.0 .8
	End

End

Condition
	On 	Time
	Time 	10

	Event
		EName 	Mallet2
		Type	start 
		At	chest
		Part	WEAPONS\Bow\TrickArrow\EMPElectricityArch1.part
		Part	WEAPONS\Bow\TrickArrow\EMPElectricityArch2.part
		Part	WEAPONS\Bow\TrickArrow\EMPElectricityArch1.part
		Part	WEAPONS\Bow\TrickArrow\EMPElectricityArch2.part
		LifeSpan	23
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

