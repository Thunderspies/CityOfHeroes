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

LifeSpan	40

########################################################


Condition

	On 	time
	Time 	10

	Event
		Ename	lift
		Type	local
	
		At	root
		bhvr V_COV\EnemyPowers\Arachnos\mu_EMP_grow.bhvr
		
		lifespan 200
#		geom    Testing_Target
	End


	Event
		Type	local
	
		At	root
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMPrings.part
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
	
		At	implode
		Part	V_COV\EnemyPowers\Arachnos\mu_EMP_Implode2a.part
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMP_Implode1a.part
		pmagnet origin
		lifespan 1
	End
end

Condition

	On 	time
	Time 	12

	Event
		Type	local
	
		At	root
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMPrings.part
	End
end
Condition

	On 	time
	Time 	14

	Event
		Type	local
	
		At	root
		Part1	V_COV\EnemyPowers\Arachnos\mu_EMPrings.part
	End
end

Condition

	On 	time
	Time 	5
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
		At	lift
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_EMP_Branches.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	2

	Event
		Type	local  
		At	lift
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_EMP_Branches.fx
		lifespan 5
	end

end

Condition

	On 	cycle
	Time 	3

	Event
		Type	local  
		At	lift
		childfx	V_COV\EnemyPowers\Arachnos\Mu_HandLightningFx\Redmu_EMP_Branches.fx
		lifespan 5
	end

end
#######################################
