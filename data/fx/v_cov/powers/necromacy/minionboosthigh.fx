#########################################################
##	Fire_Ball
##

#########################################################

FxInfo

LifeSpan	300

#########################################################


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
		LifeSpan	30

	End

	Event	

		ENAME	Base
		Type	local
		At	WepL
		
		Part1	:DarkHand.part
		Part3	:DarkHandsEmber.part
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	0

	Event
		EName 	Hand1
		Type	Local 
		At	UarmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		Sound miasma3 100 100 .8
		
		PMagnet	LarmR
		LifeSpan	30
	End

	Event
		EName 	Hand3
		Type	Local
		At	UarmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
				
		PMagnet	LarmL
		LifeSpan	30
	End

End

Condition
	On 	Time
	Time 	15

	Event
		EName 	Hand2
		Type	Local 
		At	LarmR
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		
		PMagnet	WepR
		LifeSpan	30
	End

	

	Event
		EName 	Hand4
		Type	Local
		At	LarmL
		part1	:SmokeHands.part
		part2	:SmokeHands2.part
		Part3	:DarkBlastHitTendril.part
		
		PMagnet	WepL
		LifeSpan	30
	End

End


Condition
	On 	Time
	Time 	14


	Event
		EName 	Prime
		Type	Start 
		At	Root
		Bhvr	behaviors/slowprjct5.bhvr

		part	:FlatEvilSmoke.part
		part	:FlatEvilSmoke2.part
		part	:EvilSmoke.part
		part	:EvilSmoke2.part
		Part	:EvilTendril.part
		Part	:Roaches.part
		Part	:Beatles.part
		Part	:Bees.part
		Part	:Bees.part
		Part	:Bees.part


		Magnet	T_Root
		LookAt	T_Root
						
	End

End

#Condition
#	On 	Time
#	Time 	15
#
#
#	Event
#		Ename	Prime1
#		Type	Start 
#		At	Prime
#
#		Bhvr	behaviors/slowprjct5.bhvr
#		Part	:Bones.part
#		Part	:Bones1.part
#		Part	:Bones2.part
#			
#		Magnet	T_Root
#		LookAt	T_Root			
#	End
#
#End

Condition
	On 	PrimeHit

	Event	
		Type	start
		At	T_Root
		BHVR	:Blacksplat.bhvr
		
		Splat	Ember.tga
		LifeSpan	15
	End

#################################################################################################
	
	Event
		Type	Local 
		At	T_Root
		Part	:HitEvilBeatles.part
		Part	:HitEvilRoaches.part
		part	:SmallDarkStreaks.part
		part	:DarkGhosts.part
		part	:DarkGhosts2.part
		part	:DarkGhosts3.part

		LifeSpan	50
	End
	
	Event
		Type	Local 
		At	T_Hips
		Part	:BeesHit.part
		LifeSpan	70
	End

	Event
		EName 	OrbMist
		Type	Local 
		At	T_Root
		geom	FX_RuneStreak
		bhvr	:SpinFast.bhvr
		LifeSpan	60
	End

	Event
		EName 	OrbStreak
		Type	Local 
		At	T_Root
		geom	FX_RuneStreak
		bhvr	:SpinFast2.bhvr
				
		LifeSpan	80
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbStreak
		altpiv  1
		Part	:PurpleMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
		part	:HitEvilTendril.part
		
		
	End

	Event
		EName 	Orb
		Type	Local 
		At	OrbMist
		altpiv  1
		Part	:PurpleMagic.part
		part	:HitEvilSmoke.part
		part	:HitEvilSmoke2.part
		part	:HitEvilTendril.part
			
		
	End

	Event
		EName 	Prime
		Type	Destroy

	End

End

Condition
	On 	Prime1Hit

	Event
		EName 	Prime1
		Type	Destroy

	End
End


End	


			