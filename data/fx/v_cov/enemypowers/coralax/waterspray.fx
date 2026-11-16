#########################################################
##	Fireweapon
########################################################
FxInfo
LifeSpan	80  
######################################################################


Condition

	On 	Time
	Time 	20
		Event
		Type	local
		At	head

		Sound steamblast2 100 100 .75
	end
end


Condition

	On 	Time
	Time 	20
		Event
		Type	local
		At	head

		Sound waterfountain2_loop 100 100 .66
	end
end

Condition
	On 	Time
	Time 	30

	Event
		
		Type	positonly
		At	head
		ename	header
		lookat	T_Root

	end

	Event
		Ename	Spray
		Type	local
		At	header
		BHVR	:WaterSprayOffset.bhvr
		part	:WaterFountainMedium.part
#		part	:WaterMistMedium.part
#		part	:WaterbubbleMedium.part
#		part	:WaterbubbleMedium.part
#		part	:WaterMistBright.part
		#pmagnet	T_Root
#		lookat	T_Chest

#		Sound	smallextinguishblast 100 80 .24
		
	End

	
	Event
		Type	local
		At	spray
		bhvrOverride
			PositionOffset	0 0.05 0
		End
		part	:WaterSpray_Spittle.part
		
	End


End


End
##################################

