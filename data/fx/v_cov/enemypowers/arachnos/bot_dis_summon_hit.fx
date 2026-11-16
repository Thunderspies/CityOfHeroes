#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

Input  
	Inpname	Origin
End

Input  
	Inpname	Target
End

Input  
	Inpname	Mystic
End

Input  
	Inpname	HandR
End

LifeSpan	80

########################################################



#Condition
#
#	On 	time
#	Time 	10
#
#	Event
#		Ename	lift
#		Type	local
#	
#		At	root
#		bhvr :mu_EMP_grow.bhvr
#		
#		lifespan 200
##		geom    Testing_Target
#	End
#
#
#	Event
#		Type	local
#	
#		At	root
#		Part1	:mu_EMPrings.part
#	End
#	Event
#		ename implode
#		Type	local
#	
#		At	lift
#		bhvr :mu_EMP_growSpin.bhvr
#		lifespan 13
#	End
#end

#Condition
#
#	On 	time
#	Time 	5
#event
#		Type	positonly
#		At	Hips
#		Sound	eleccontrol2 100 30 .5
#end
#
#end

Condition

## screws

	On 	time
	Time 	10
	Event
		Type	local
		At	C_HandR
		bhvr	:Bot_Dis_Smn_R.bhvr
		part1	:Bot_Dis_Summon_Rings.part
		lifespan 25
	End
	Event
		Type	local
		At	C_HandL
		bhvr	:Bot_Dis_Smn_L.bhvr
		part1	:Bot_Dis_Summon_Rings.part
		lifespan 25
	End
End




Condition

	On 	time
	Time 	20
	Event
		Type	local
		At	C_FootL
		bhvr	:Bot_Dis_Smn_B.bhvr
		part1	:Bot_Dis_Summon_Rings.part
		lifespan 25
	End
	Event
		Type	local
		At	C_FootR
		bhvr	:Bot_Dis_Smn_B.bhvr
		part1	:Bot_Dis_Summon_Rings.part
		lifespan 25
	End
End

############ vibrating legs


Condition

	On 	time
	Time 	17

## toes

	Event
		Type	local
		At	Fore_FootL
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 Fore_ToeL
	End

	Event
		Type	local
		At	LarmL
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 HandL
	End

	Event
		Type	local
		At	Lwing3
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 Lwing4
	End

	Event
		Type	local
		At	FootL
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 ToeL
	End



	Event
		Type	local
		At	Fore_FootR
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 Fore_ToeR
	End

	Event
		Type	local
		At	LarmR
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 HandR
	End

	Event
		Type	local
		At	Rwing3
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 Rwing4
	End

	Event
		Type	local
		At	FootR
		part1	:Bot_Dit_Smn_vibe.part
		lifespan 31
		pother	 ToeR
	End


End

Condition

	On 	time
	Time 	10

## legs

	Event
		Type	local
		At	Fore_LLegL
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 Fore_FootL
	End

	Event
		Type	local
		At	UarmL
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 LarmL
	End

	Event
		Type	local
		At	Lwing2
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 Lwing3
	End

	Event
		Type	local
		At	LlegL
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 FootL
	End



	Event
		Type	local
		At	Fore_LLegR
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 Fore_FootR
	End

	Event
		Type	local
		At	UarmR
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 LarmR
	End

	Event
		Type	local
		At	Rwing2
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 Rwing3
	End

	Event
		Type	local
		At	LlegR
		part1	:Bot_Dit_Smn_vibeDK.part
		lifespan 31
		pother	 FootR
	End


end


#Condition
#
#	On 	cycle
#	Time 	1
#
#	Event
#		Type	local  
#		At	lift
#		childfx	:mu_EMP_Branches.fx
#		lifespan 5
#	end
#
#end
#
#Condition
#
#	On 	cycle
#	Time 	2
#
#	Event
#		Type	local  
#		At	lift
#		childfx	:mu_EMP_Branches.fx
#		lifespan 5
#	end
#
#end
#
#Condition
#
#	On 	cycle
#	Time 	3
#
#	Event
#		Type	local  
#		At	lift
#		childfx	:mu_EMP_Branches.fx
#		lifespan 5
#	end
#
#end
#######################################
