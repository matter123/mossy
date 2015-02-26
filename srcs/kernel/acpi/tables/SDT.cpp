/*
	Copyright 2015 Matthew Fosdick

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

	http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include <acpi/tables/SDT.h>
#include <acpi/sdt.h>
#include <string.h>
namespace acpi {
	SDT::SDT(pointer s) {
		sdt *table=reinterpret_cast<sdt *>(s);
		memcpy(this->signature,table->Signature,4);
		this->signature[4]='\0';
		this->revision=table->revision;
		memcpy(this->OEM_ID,table->OEMID,6);
		this->OEM_ID[6]='\0';
		memcpy(this->OEM_TABLE_ID,table->OEMTableID,8);
		this->OEM_TABLE_ID[8]='\0';
		this->OEM_revision=table->OEMrevision;
		this->creator_ID=table->creatorID;
		this->creator_revision=table->creator_revision;
		this->phys_table_addr=reinterpret_cast<void *>(s);
	}
}
