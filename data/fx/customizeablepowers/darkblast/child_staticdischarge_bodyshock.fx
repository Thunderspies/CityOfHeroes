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
	Time 	0
	Event
		Type	local

		At	UarmR
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother LarmR
		pmagnet UarmR
	end

	Event
		Type	local

		At	LarmR
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother HandR
		pmagnet LarmR
	end

	Event
		Type	local

		At	UarmL
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother LarmL
		pmagnet UarmL
	end

	Event
		Type	local

		At	LarmL
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		Pother HandL
		pmagnet LarmL
	end

	Event
		Type	local

		At	UlegR
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother LlegR
		pmagnet UlegR
	end

	Event
		Type	local

		At	LlegR
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother FootR
		pmagnet LlegR
	end

	Event
		Type	local

		At	UlegL
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother LlegL
		pmagnet UlegL
	end

	Event
		Type	local

		At	LlegL
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock1.part
		part1	V_COV\EnemyPowers\Arachnos\mu_Elec_BodyShock2.part
		Pother FootL
		pmagnet LlegL
	end

end


