################x#########################################
##	chill touch hit
FxInfo


LifeSpan	75

############################################################


Condition
	On 	Time
	Time 	15

	Event	

		ENAME	skull
		Type	local
		At	head
	##	Sound	aura4 80 30 .8
		Sound ScreamPBAOE 80 80 .8
		
	End

End



Condition
	On 	Time
	Time 	30

	Event	

		ENAME	skull
		Type	local
		At	head
		Anim	Fx_Skull
		bhvr	behaviors/Caleb_SkullAlpha.bhvr
		Sound aura4 80 80 .8
		Lifespan 20
		
	End

End

#Condition
#	On 	Time
#	Time 	40
#
#	Event	
#
#		ENAME	skull2
#		Type	local
#		At	head
#		Anim	Fx_Skull
#		bhvr	behaviors/Caleb_SkullAlpha.bhvr
#		Lifespan 10
#		
#	End
#
#End
#
#
#Condition
#	On 	Time
#	Time 	50
#
#	Event	
#
#		ENAME	skull2
#		Type	local
#		At	head
#		Anim	Fx_Skull
#		bhvr	behaviors/Caleb_SkullAlpha.bhvr
#		Lifespan 10
#		
#	End
#
#End



Condition
	On 	Time
	Time 	20

	
	Event
		EName 	Orb
		Type	Local 
		At	head		
		part1	:Caleb_FearMist.part
		part2	:Caleb_FearMist2.part
		Part3	:FearTenticles.part
		Part4	:FearTenticles2.part
		Lifespan 30
		
	End

End	




End			