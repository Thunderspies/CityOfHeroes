#########################################################
##	General red electricity crawling across characters limbs
########################################################
FxInfo

LifeSpan	142

########################################################

#########################################################

Condition

	On 	Time
	Time 	55

	Event
		Ename	StartCycle
		Type	local  
		At	WepR
	
		LifeSpan	42
	end

end

#########################################################
##Flash
####################################

Condition

	On 	time
	Time 	60

	Event

		Ename	Flash
		Type	start
		At	C_HandL	

		Part	:OverChargeFlash.part
		Part	:Rays.part
		lifespan 65
	End
	
	Event

		Ename	Flash
		Type	start
		At	C_HandR	

		Part	:OverChargeFlash.part
		Part	:Rays.part
		lifespan 65
	End

End

Condition

	Event

		Ename	OverloadCore
		Type	local
		At	Origin	
		lifespan 65

	End
End

Condition
	On 	cycle
	Time 	3
	
	Event
		Type	local
		At	OverloadCore
		childfx :OverloadWindup.fx
		lifespan 10
	end
End



############
Condition

	On 	time
	Time 	10

	Event
		Ename	lift
		Type	local
	
		At	root
		bhvr V_COV\EnemyPowers\Arachnos\mu_EMP_grow.bhvr
		
	
		LifeSpan	42
#		geom    Testing_Target
	End

	Event
		ename implode
		Type	local
	
		At	lift
		bhvr V_COV\EnemyPowers\Arachnos\mu_EMP_growSpin.bhvr
		lifespan 13
	End
end

Condition

	On 	cycle
	Time 	1
	Event
		Type	local
	
		At	StartCycle
		Part	V_COV\EnemyPowers\Arachnos\mu_EMP_Implode2purple.part
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMP_Implodepurple.part
		pmagnet origin
		lifespan 1
	End
end

################################################################
##Rings
###################################3

Condition

	On 	time
	Time 	60

	Event
		Type	local
	
		At	root
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMPringspurple.part
	
		LifeSpan	42
	End
end

Condition

	On 	time
	Time 	62

	Event
		Type	local
	
		At	root
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMPringspurple.part
	
		LifeSpan	42
	End
end

#############################################################################################

Condition

	On 	time
	Time 	5
	
	event
		Type	positonly
		At	Hips
		Sound staticdischarge2 100 100 .7
	end

end



Condition

	On 	time
	Time 	55
	
	event
		Type	positonly
		At	Hips
		Sound mu_emp 100 100 1.0
	end

end

Condition

	On 	cycle
	Time 	1

	Event
		Type	local  
		At	StartCycle
		childfx	V_COV\EnemyPowers\Arachnos\mu_EMP_BranchesPurple.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	2

	Event
		Type	local  
		At	StartCycle
		childfx	V_COV\EnemyPowers\Arachnos\mu_EMP_BranchesPurple.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	3

	Event
		Type	local  
		At	StartCycle
		childfx	V_COV\EnemyPowers\Arachnos\mu_EMP_BranchesPurple.fx
		lifespan 5
	end

end

end
###########################
