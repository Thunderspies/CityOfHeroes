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

#LifeSpan	40

########################################################


Condition

	On 	cycle
	Time 	9
	Event
		Type	local 
	
		At	UarmR
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother LarmR
		pmagnet UarmR
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	LarmR
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother HandR
		pmagnet LarmR
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	UarmL
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother LarmL
		pmagnet UarmL
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	LarmL
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother HandL
		pmagnet LarmL
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	UlegR
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother LlegR
		pmagnet UlegR
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	LlegR
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother FootR
		pmagnet LlegR
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	UlegL
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother LlegL
		pmagnet UlegL
		LifeSpan 40
	end

	Event
		Type	local 
	
		At	LlegL
		part1	:mu_Elec_BodyShock1.part
		part1	:mu_Elec_BodyShock2.part
		Pother FootL
		pmagnet LlegL
		LifeSpan 40
	end

end


